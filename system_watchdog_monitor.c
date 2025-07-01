/*
 * System Watchdog Monitor using SUSI API
 * Monitors CPU, Memory, and Network activity
 * Automatically reboots device if no activity detected
 */

 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/timerfd.h>
 
 // SUSI includes - adjust path as needed
 #include "Susi4.h"
 
 // Default configuration values
#define DEFAULT_WATCHDOG_TIMEOUT    60      // Watchdog timeout in seconds
#define DEFAULT_MAX_INACTIVE_CYCLES 3       // Max cycles without activity before allowing reboot
#define DEFAULT_CPU_THRESHOLD       5.0     // 5% CPU usage
#define DEFAULT_MAX_CPU_THRESHOLD   90.0    // 90% CPU usage - restart if exceeded
#define DEFAULT_MEM_THRESHOLD       1024    // 1KB memory change
#define DEFAULT_NET_THRESHOLD       100     // 100 bytes network activity
#define DEFAULT_CONFIG_FILE         "/etc/system_watchdog_monitor.conf"
#define DEFAULT_LOG_FILE            "/var/log/system_watchdog_monitor.log"
#define DEFAULT_LOG_ENABLED         1       // Enable logging by default

// Real-time monitoring settings
#define DEFAULT_CPU_CHECK_INTERVAL  1       // Check CPU every 1 second
#define DEFAULT_MEM_CHECK_INTERVAL  2       // Check memory every 2 seconds
#define DEFAULT_NET_CHECK_INTERVAL  1       // Check network every 1 second
#define MAX_EPOLL_EVENTS           10       // Maximum number of epoll events to process at once
 
 // Runtime configuration
typedef struct {
    int watchdog_timeout;
    int max_inactive_cycles;
    double cpu_threshold;
    double max_cpu_threshold;
    unsigned long long mem_threshold;
    unsigned long long net_threshold;
    char config_file[256];
    char log_file[256];            // Path to log file
    int log_enabled;              // Whether to write logs to file
    int cpu_check_interval;        // How often to check CPU (seconds)
    int mem_check_interval;        // How often to check memory (seconds)
    int net_check_interval;        // How often to check network (seconds)
} WatchdogConfig;
 
 // Global configuration
static WatchdogConfig g_config = {
    .watchdog_timeout = DEFAULT_WATCHDOG_TIMEOUT,
    .max_inactive_cycles = DEFAULT_MAX_INACTIVE_CYCLES,
    .cpu_threshold = DEFAULT_CPU_THRESHOLD,
    .max_cpu_threshold = DEFAULT_MAX_CPU_THRESHOLD,
    .mem_threshold = DEFAULT_MEM_THRESHOLD,
    .net_threshold = DEFAULT_NET_THRESHOLD,
    .config_file = DEFAULT_CONFIG_FILE,
    .log_file = DEFAULT_LOG_FILE,
    .log_enabled = DEFAULT_LOG_ENABLED,
    .cpu_check_interval = DEFAULT_CPU_CHECK_INTERVAL,
    .mem_check_interval = DEFAULT_MEM_CHECK_INTERVAL,
    .net_check_interval = DEFAULT_NET_CHECK_INTERVAL
};
 
 // Global variables
static int g_running = 1;
static int g_inactive_cycles = 0;
static SusiId_t g_watchdog_id = SUSI_ID_WATCHDOG_1;
static FILE *g_log_file = NULL;  // Log file handle
static time_t g_last_detailed_log = 0;  // Last time detailed activity was logged
static unsigned long g_watchdog_feeds = 0;  // Count of watchdog feeds

// Real-time monitoring variables
static int g_epoll_fd = -1;           // Epoll file descriptor
static int g_cpu_timer_fd = -1;       // Timer fd for CPU monitoring
static int g_mem_timer_fd = -1;       // Timer fd for memory monitoring
static int g_net_timer_fd = -1;       // Timer fd for network monitoring
static int g_activity_detected = 0;   // Flag to track if any activity detected
 
 // Previous system stats for comparison
 static struct {
     unsigned long long cpu_total;
     unsigned long long cpu_idle;
     unsigned long long mem_available;
     unsigned long long net_rx_bytes;
     unsigned long long net_tx_bytes;
     time_t last_check;
 } g_prev_stats = {0};
 
 // Function prototypes
void signal_handler(int sig);
int init_susi_watchdog(void);
void cleanup_susi_watchdog(void);
int get_cpu_usage(double *cpu_percent);
int get_memory_usage(unsigned long long *mem_available);
int get_network_activity(unsigned long long *rx_bytes, unsigned long long *tx_bytes);
int check_system_activity(int event_type);
void log_message(const char *message);
int feed_watchdog(void);
void parse_args(int argc, char *argv[]);

// Real-time monitoring functions
int init_realtime_monitoring(void);
void cleanup_realtime_monitoring(void);
int create_timer_fd(int interval_seconds);
void handle_timer_event(int timer_fd);
void reset_activity_flag(void);
 
 // Signal handler for graceful shutdown
 void signal_handler(int sig) {
     (void)sig; // Suppress unused parameter warning
     g_running = 0;
     printf("Signal received, shutting down...\n");
 }
 
 // Initialize SUSI library and start watchdog
 int init_susi_watchdog(void) {
     SusiStatus_t status;
     
     // Initialize SUSI library
     status = SusiLibInitialize();
     if (status != SUSI_STATUS_SUCCESS) {
         fprintf(stderr, "Failed to initialize SUSI library: 0x%08X\n", status);
         return -1;
     }
     
     // Start watchdog with correct parameters:
     // DelayTime=0 (start immediately), EventTime=0 (no warning event), 
     // ResetTime=watchdog_timeout, EventType=NONE
     status = SusiWDogStart(g_watchdog_id, 0, 0, g_config.watchdog_timeout, SUSI_WDT_EVENT_TYPE_NONE);
     if (status != SUSI_STATUS_SUCCESS) {
         fprintf(stderr, "Failed to start watchdog: 0x%08X\n", status);
         SusiLibUninitialize();
         return -1;
     }
     
     printf("Watchdog started with reset timeout: %d seconds\n", g_config.watchdog_timeout);
     return 0;
 }
 
 // Cleanup SUSI watchdog
 void cleanup_susi_watchdog(void) {
     SusiStatus_t status;
     
     // Stop watchdog
     status = SusiWDogStop(g_watchdog_id);
     if (status != SUSI_STATUS_SUCCESS) {
         printf("Warning: Failed to stop watchdog (0x%08X)\n", status);
     } else {
         log_message("Watchdog stopped successfully");
     }
     
     // Uninitialize SUSI library
     SusiLibUninitialize();
 }
 
 // Feed the watchdog to prevent reset
 int feed_watchdog(void) {
     SusiStatus_t status = SusiWDogTrigger(g_watchdog_id);
     if (status != SUSI_STATUS_SUCCESS) {
         log_message("ERROR: Failed to feed watchdog");
         return -1;
     }
     return 0;
 }
 
 // Get CPU usage percentage
 int get_cpu_usage(double *cpu_percent) {
     FILE *fp;
     char buffer[256];
     unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
     unsigned long long total, total_diff, idle_diff;
     
     fp = fopen("/proc/stat", "r");
     if (!fp) {
         perror("Failed to open /proc/stat");
         return -1;
     }
     
     if (fgets(buffer, sizeof(buffer), fp) == NULL) {
         fclose(fp);
         return -1;
     }
     fclose(fp);
     
     // Parse CPU stats
     if (sscanf(buffer, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
                &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal) != 8) {
         return -1;
     }
     
     total = user + nice + system + idle + iowait + irq + softirq + steal;
     
     if (g_prev_stats.cpu_total > 0) {
         total_diff = total - g_prev_stats.cpu_total;
         idle_diff = idle - g_prev_stats.cpu_idle;
         *cpu_percent = (total_diff > 0) ? (100.0 * (total_diff - idle_diff) / total_diff) : 0.0;
     } else {
         *cpu_percent = 0.0;
     }
     
     g_prev_stats.cpu_total = total;
     g_prev_stats.cpu_idle = idle;
     
     return 0;
 }
 
 // Get available memory
 int get_memory_usage(unsigned long long *mem_available) {
     FILE *fp;
     char buffer[256];
     
     fp = fopen("/proc/meminfo", "r");
     if (!fp) {
         perror("Failed to open /proc/meminfo");
         return -1;
     }
     
     while (fgets(buffer, sizeof(buffer), fp)) {
         if (sscanf(buffer, "MemAvailable: %llu kB", mem_available) == 1) {
             fclose(fp);
             return 0;
         }
     }
     
     fclose(fp);
     return -1;
 }
 
 // Get network activity
 int get_network_activity(unsigned long long *rx_bytes, unsigned long long *tx_bytes) {
     FILE *fp;
     char buffer[256];
     char interface[32];
     unsigned long long rx, tx;
     
     *rx_bytes = 0;
     *tx_bytes = 0;
     
     fp = fopen("/proc/net/dev", "r");
     if (!fp) {
         perror("Failed to open /proc/net/dev");
         return -1;
     }
     
     // Skip header lines
     fgets(buffer, sizeof(buffer), fp);
     fgets(buffer, sizeof(buffer), fp);
     
     // Sum up all interfaces except loopback
     while (fgets(buffer, sizeof(buffer), fp)) {
         if (sscanf(buffer, "%31[^:]: %llu %*u %*u %*u %*u %*u %*u %*u %llu",
                    interface, &rx, &tx) == 3) {
             if (strcmp(interface, "lo") != 0) {  // Skip loopback
                 *rx_bytes += rx;
                 *tx_bytes += tx;
             }
         }
     }
     
     fclose(fp);
     return 0;
 }
 
 // Check if there's system activity based on event type
int check_system_activity(int event_type) {
    double cpu_percent = 0.0;
    unsigned long long mem_available = 0, net_rx = 0, net_tx = 0;
    int activity_detected = 0;
    int cpu_active = 0, mem_active = 0, net_active = 0;
    int cpu_critical = 0;  // Flag for max CPU threshold exceeded
    time_t current_time = time(NULL);
    char timestamp[64];
    struct tm *tm_info;
    char log_buffer[512] = {0};
    
    time(&current_time);
    tm_info = localtime(&current_time);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Process events based on timer type
    if (event_type == g_cpu_timer_fd) {
        // CPU monitoring event
        if (get_cpu_usage(&cpu_percent) == 0) {
            // Check for critical CPU usage (exceeds max threshold)
            if (cpu_percent > g_config.max_cpu_threshold) {
                snprintf(log_buffer, sizeof(log_buffer), "CRITICAL: CPU usage %.2f%% exceeds maximum threshold %.2f%%!", 
                         cpu_percent, g_config.max_cpu_threshold);
                log_message(log_buffer);
                cpu_critical = 1;
            }
            // Check for normal CPU activity (above min threshold)
            else if (cpu_percent > g_config.cpu_threshold) {
                if (current_time - g_last_detailed_log >= 30) {
                    snprintf(log_buffer, sizeof(log_buffer), "CPU activity: %.2f%% (threshold: %.2f%%, max: %.2f%%)", 
                             cpu_percent, g_config.cpu_threshold, g_config.max_cpu_threshold);
                    log_message(log_buffer);
                }
                activity_detected = 1;
                cpu_active = 1;
            }
        }
    } else if (event_type == g_mem_timer_fd) {
        // Memory monitoring event
        if (get_memory_usage(&mem_available) == 0) {
            static long long prev_mem_available = 0;
            long long mem_diff = mem_available - prev_mem_available;
            
            // Use llabs for long long absolute value
            if (llabs(mem_diff) > g_config.mem_threshold) {
                if (current_time - g_last_detailed_log >= 30) {
                    snprintf(log_buffer, sizeof(log_buffer), "Memory activity: %lld bytes change (threshold: %llu)", 
                             mem_diff, g_config.mem_threshold);
                    log_message(log_buffer);
                }
                activity_detected = 1;
                mem_active = 1;
            }
            prev_mem_available = mem_available;
        }
    } else if (event_type == g_net_timer_fd) {
        // Network monitoring event
        if (get_network_activity(&net_rx, &net_tx) == 0) {
            if (g_prev_stats.net_rx_bytes > 0 && g_prev_stats.net_tx_bytes > 0) {
                unsigned long long rx_diff = net_rx - g_prev_stats.net_rx_bytes;
                unsigned long long tx_diff = net_tx - g_prev_stats.net_tx_bytes;
                if (rx_diff > g_config.net_threshold || tx_diff > g_config.net_threshold) {
                    if (current_time - g_last_detailed_log >= 30) {
                        snprintf(log_buffer, sizeof(log_buffer), 
                                "Network activity: RX:%llu TX:%llu bytes (threshold: %llu)", 
                                rx_diff, tx_diff, g_config.net_threshold);
                        log_message(log_buffer);
                    }
                    activity_detected = 1;
                    net_active = 1;
                }
            }
            g_prev_stats.net_rx_bytes = net_rx;
            g_prev_stats.net_tx_bytes = net_tx;
        }
    }
    
    // If activity detected, set global flag and feed watchdog immediately
    if (activity_detected) {
        g_activity_detected = 1;
        g_inactive_cycles = 0;  // Reset inactive cycles on activity
        
        // Feed watchdog immediately when activity is detected
        if (feed_watchdog() == 0) {
            g_watchdog_feeds++;
            
            // Detailed log every 30 seconds or first feed
            if (current_time - g_last_detailed_log >= 30 || g_watchdog_feeds == 1) {
                snprintf(log_buffer, sizeof(log_buffer), "Activity detected - watchdog fed #%lu [CPU:%s MEM:%s NET:%s]",
                       g_watchdog_feeds,
                       cpu_active ? "active" : "idle",
                       mem_active ? "active" : "idle",
                       net_active ? "active" : "idle");
                log_message(log_buffer);
                g_last_detailed_log = current_time;
            } else if (g_watchdog_feeds % 6 == 0) {
                // Brief log every few feeds
                snprintf(log_buffer, sizeof(log_buffer), "Watchdog fed #%lu - system healthy", g_watchdog_feeds);
                log_message(log_buffer);
            }
        } else {
            log_message("Error feeding watchdog after activity detection!");
            return -2;  // Watchdog feed error
        }
    }
    
    g_prev_stats.last_check = current_time;
    
    // Return special code for critical CPU condition
    if (cpu_critical) {
        return -1;  // Critical condition - stop feeding watchdog
    }
    
    return activity_detected;
}

// Create a timer file descriptor with specified interval
int create_timer_fd(int interval_seconds) {
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timer_fd == -1) {
        log_message("Failed to create timer fd");
        return -1;
    }
    
    struct itimerspec timer_spec = {0};
    timer_spec.it_value.tv_sec = interval_seconds;  // Initial expiration
    timer_spec.it_interval.tv_sec = interval_seconds;  // Periodic expiration
    
    if (timerfd_settime(timer_fd, 0, &timer_spec, NULL) == -1) {
        log_message("Failed to set timer");
        close(timer_fd);
        return -1;
    }
    
    return timer_fd;
}

// Initialize real-time monitoring system
int init_realtime_monitoring(void) {
    // Create epoll instance
    g_epoll_fd = epoll_create1(0);
    if (g_epoll_fd == -1) {
        log_message("Failed to create epoll instance");
        return -1;
    }
    
    // Create and add CPU timer
    g_cpu_timer_fd = create_timer_fd(g_config.cpu_check_interval);
    if (g_cpu_timer_fd == -1) {
        log_message("Failed to create CPU timer");
        close(g_epoll_fd);
        return -1;
    }
    
    struct epoll_event ev = {0};
    ev.events = EPOLLIN;
    ev.data.fd = g_cpu_timer_fd;
    if (epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_cpu_timer_fd, &ev) == -1) {
        log_message("Failed to add CPU timer to epoll");
        close(g_cpu_timer_fd);
        close(g_epoll_fd);
        return -1;
    }
    
    // Create and add memory timer
    g_mem_timer_fd = create_timer_fd(g_config.mem_check_interval);
    if (g_mem_timer_fd == -1) {
        log_message("Failed to create memory timer");
        close(g_cpu_timer_fd);
        close(g_epoll_fd);
        return -1;
    }
    
    ev.data.fd = g_mem_timer_fd;
    if (epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_mem_timer_fd, &ev) == -1) {
        log_message("Failed to add memory timer to epoll");
        close(g_mem_timer_fd);
        close(g_cpu_timer_fd);
        close(g_epoll_fd);
        return -1;
    }
    
    // Create and add network timer
    g_net_timer_fd = create_timer_fd(g_config.net_check_interval);
    if (g_net_timer_fd == -1) {
        log_message("Failed to create network timer");
        close(g_mem_timer_fd);
        close(g_cpu_timer_fd);
        close(g_epoll_fd);
        return -1;
    }
    
    ev.data.fd = g_net_timer_fd;
    if (epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, g_net_timer_fd, &ev) == -1) {
        log_message("Failed to add network timer to epoll");
        close(g_net_timer_fd);
        close(g_mem_timer_fd);
        close(g_cpu_timer_fd);
        close(g_epoll_fd);
        return -1;
    }
    
    log_message("Real-time monitoring system initialized");
    return 0;
}

// Clean up real-time monitoring resources
void cleanup_realtime_monitoring(void) {
    if (g_cpu_timer_fd != -1) {
        close(g_cpu_timer_fd);
        g_cpu_timer_fd = -1;
    }
    
    if (g_mem_timer_fd != -1) {
        close(g_mem_timer_fd);
        g_mem_timer_fd = -1;
    }
    
    if (g_net_timer_fd != -1) {
        close(g_net_timer_fd);
        g_net_timer_fd = -1;
    }
    
    if (g_epoll_fd != -1) {
        close(g_epoll_fd);
        g_epoll_fd = -1;
    }
    
    log_message("Real-time monitoring system cleaned up");
}

// Handle timer event - read timer to clear it and perform checks
void handle_timer_event(int timer_fd) {
    uint64_t expirations;
    if (read(timer_fd, &expirations, sizeof(expirations)) == -1) {
        // Just to clear the timer, ignore errors
        return;
    }
}

// Reset activity detection flag (now handled inline in main loop)
void reset_activity_flag(void) {
    g_activity_detected = 0;
}

// Log messages with timestamp
void log_message(const char *message) {
    time_t now;
    struct tm *tm_info;
    char timestamp[64];
    
    time(&now);
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Print to console
    printf("[%s] %s\n", timestamp, message);
    fflush(stdout);
    
    // Write to log file if enabled
    if (g_log_file) {
        fprintf(g_log_file, "[%s] %s\n", timestamp, message);
        fflush(g_log_file);
    }
}

// Print usage information
void print_usage(const char *program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  -c, --config FILE     Use configuration file (default: %s)\n", DEFAULT_CONFIG_FILE);
    printf("  -w, --timeout SECS    Set watchdog timeout in seconds (default: %d)\n", DEFAULT_WATCHDOG_TIMEOUT);
    printf("  -i, --inactive NUM    Set max inactive cycles before reboot (default: %d)\n", DEFAULT_MAX_INACTIVE_CYCLES);
    printf("  -p, --cpu PERCENT     Set CPU activity threshold percentage (default: %.1f)\n", DEFAULT_CPU_THRESHOLD);
    printf("  -x, --max-cpu PERCENT Set maximum CPU threshold for restart (default: %.1f)\n", DEFAULT_MAX_CPU_THRESHOLD);
    printf("  -e, --memory BYTES    Set memory change threshold in bytes (default: %d)\n", DEFAULT_MEM_THRESHOLD);
    printf("  -n, --network BYTES   Set network activity threshold in bytes (default: %d)\n", DEFAULT_NET_THRESHOLD);
    printf("      --cpu-interval S  Set CPU check interval in seconds (default: %d)\n", DEFAULT_CPU_CHECK_INTERVAL);
    printf("      --mem-interval S  Set memory check interval in seconds (default: %d)\n", DEFAULT_MEM_CHECK_INTERVAL);
    printf("      --net-interval S  Set network check interval in seconds (default: %d)\n", DEFAULT_NET_CHECK_INTERVAL);
    printf("  -l, --log-file FILE   Set log file path (default: %s)\n", DEFAULT_LOG_FILE);
    printf("  -d, --disable-log     Disable writing to log file\n");
    printf("  -h, --help            Display this help message\n");
}

// Load configuration from file
int load_config_from_file(const char *filename) {
    FILE *config_file = fopen(filename, "r");
    if (!config_file) {
        return -1;
    }

    char line[256];
    char name[64];
    char value[64];

    while (fgets(line, sizeof(line), config_file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        if (sscanf(line, "%63[^=]=%63s", name, value) == 2) {
            // Remove whitespace
            char *p = name + strlen(name) - 1;
            while (p >= name && (*p == ' ' || *p == '\t')) {
                *p-- = '\0';
            }

            if (strcmp(name, "watchdog_timeout") == 0) {
                g_config.watchdog_timeout = atoi(value);
            } else if (strcmp(name, "max_inactive_cycles") == 0) {
                g_config.max_inactive_cycles = atoi(value);
            } else if (strcmp(name, "cpu_threshold") == 0) {
                g_config.cpu_threshold = atof(value);
            } else if (strcmp(name, "max_cpu_threshold") == 0) {
                g_config.max_cpu_threshold = atof(value);
            } else if (strcmp(name, "mem_threshold") == 0) {
                g_config.mem_threshold = strtoull(value, NULL, 10);
            } else if (strcmp(name, "net_threshold") == 0) {
                g_config.net_threshold = strtoull(value, NULL, 10);
            } else if (strcmp(name, "log_file") == 0) {
                strncpy(g_config.log_file, value, sizeof(g_config.log_file) - 1);
            } else if (strcmp(name, "log_enabled") == 0) {
                g_config.log_enabled = atoi(value);
            } else if (strcmp(name, "cpu_check_interval") == 0) {
                g_config.cpu_check_interval = atoi(value);
            } else if (strcmp(name, "mem_check_interval") == 0) {
                g_config.mem_check_interval = atoi(value);
            } else if (strcmp(name, "net_check_interval") == 0) {
                g_config.net_check_interval = atoi(value);
            }
        }
    }

    fclose(config_file);
    return 0;
}

// Parse command line arguments
void parse_args(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"config",     required_argument, 0, 'c'},
        {"timeout",    required_argument, 0, 'w'},
        {"inactive",   required_argument, 0, 'i'},
        {"cpu",        required_argument, 0, 'p'},
        {"max-cpu",    required_argument, 0, 'x'},
        {"memory",     required_argument, 0, 'e'},
        {"network",    required_argument, 0, 'n'},
        {"cpu-interval", required_argument, 0, 0},
        {"mem-interval", required_argument, 0, 0},
        {"net-interval", required_argument, 0, 0},
        {"log-file",   required_argument, 0, 'l'},
        {"disable-log", no_argument,      0, 'd'},
        {"help",       no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    // First, check for explicit help flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            exit(0);
        }
    }

    // First, try loading from default config file
    if (access(DEFAULT_CONFIG_FILE, R_OK) == 0) {
        if (load_config_from_file(DEFAULT_CONFIG_FILE) == 0) {
            printf("Loaded configuration from %s\n", DEFAULT_CONFIG_FILE);
        }
    }

    // Then parse command-line arguments (override config file)
    int opt;
    int option_index = 0;
    optind = 0; // Reset getopt

    while ((opt = getopt_long(argc, argv, "c:w:i:p:x:e:n:l:dh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 0: // Handle long options without short equivalents
                if (strcmp(long_options[option_index].name, "cpu-interval") == 0) {
                    g_config.cpu_check_interval = atoi(optarg);
                } else if (strcmp(long_options[option_index].name, "mem-interval") == 0) {
                    g_config.mem_check_interval = atoi(optarg);
                } else if (strcmp(long_options[option_index].name, "net-interval") == 0) {
                    g_config.net_check_interval = atoi(optarg);
                }
                break;
            case 'c':
                strncpy(g_config.config_file, optarg, sizeof(g_config.config_file) - 1);
                if (load_config_from_file(g_config.config_file) == 0) {
                    printf("Loaded configuration from %s\n", g_config.config_file);
                } else {
                    fprintf(stderr, "Failed to load configuration from %s\n", g_config.config_file);
                }
                break;
            case 'w':
                g_config.watchdog_timeout = atoi(optarg);
                break;
            case 'i':
                g_config.max_inactive_cycles = atoi(optarg);
                break;
            case 'p':
                g_config.cpu_threshold = atof(optarg);
                break;
            case 'x':
                g_config.max_cpu_threshold = atof(optarg);
                break;
            case 'e':
                g_config.mem_threshold = strtoull(optarg, NULL, 10);
                break;
            case 'n':
                g_config.net_threshold = strtoull(optarg, NULL, 10);
                break;
            case 'l':
                strncpy(g_config.log_file, optarg, sizeof(g_config.log_file) - 1);
                break;
            case 'd':
                g_config.log_enabled = 0;
                break;
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case '?':
            default:
                print_usage(argv[0]);
                exit(1);
        }
    }

    // Validate configuration
    if (g_config.watchdog_timeout < 10) {
        fprintf(stderr, "Warning: Watchdog timeout too low, setting to minimum of 10 seconds\n");
        g_config.watchdog_timeout = 10;
    }
    if (g_config.max_inactive_cycles < 1) {
        fprintf(stderr, "Warning: Invalid max inactive cycles, setting to minimum of 1\n");
        g_config.max_inactive_cycles = 1;
    }
    if (g_config.max_cpu_threshold <= g_config.cpu_threshold) {
        fprintf(stderr, "Warning: Max CPU threshold (%.1f%%) must be greater than min CPU threshold (%.1f%%), setting to %.1f%%\n",
                g_config.max_cpu_threshold, g_config.cpu_threshold, g_config.cpu_threshold + 50.0);
        g_config.max_cpu_threshold = g_config.cpu_threshold + 50.0;
    }
    if (g_config.max_cpu_threshold > 100.0) {
        fprintf(stderr, "Warning: Max CPU threshold too high, setting to 100%%\n");
        g_config.max_cpu_threshold = 100.0;
    }
    // Validate real-time check intervals
    if (g_config.cpu_check_interval < 1) {
        fprintf(stderr, "Warning: CPU check interval too low, setting to minimum of 1 second\n");
        g_config.cpu_check_interval = 1;
    }
    if (g_config.mem_check_interval < 1) {
        fprintf(stderr, "Warning: Memory check interval too low, setting to minimum of 1 second\n");
        g_config.mem_check_interval = 1;
    }
    if (g_config.net_check_interval < 1) {
        fprintf(stderr, "Warning: Network check interval too low, setting to minimum of 1 second\n");
        g_config.net_check_interval = 1;
    }
}

// Initialize log file
int init_log_file(void) {
    if (!g_config.log_enabled) {
        return 0; // Logging disabled
    }

    // Open log file for append
    g_log_file = fopen(g_config.log_file, "a");
    if (!g_log_file) {
        fprintf(stderr, "Warning: Failed to open log file %s: %s\n",
                g_config.log_file, strerror(errno));
        return -1;
    }

    return 0;
}

// Close log file
void close_log_file(void) {
    if (g_log_file) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
}

int main(int argc, char *argv[]) {
    // Parse command-line arguments - do this first thing
    parse_args(argc, argv);

    // Initialize log file
    init_log_file();

    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Starting System Watchdog Monitor\n");
    printf("Configuration:\n");
    printf("  Watchdog timeout: %d seconds\n", g_config.watchdog_timeout);
    printf("  Max inactive cycles: %d\n", g_config.max_inactive_cycles);
    printf("  CPU threshold: %.1f%%\n", g_config.cpu_threshold);
    printf("  Max CPU threshold: %.1f%% (restart if exceeded)\n", g_config.max_cpu_threshold);
    printf("  Memory threshold: %llu bytes\n", g_config.mem_threshold);
    printf("  Network threshold: %llu bytes\n", g_config.net_threshold);
    printf("  CPU check interval: %d seconds\n", g_config.cpu_check_interval);
    printf("  Memory check interval: %d seconds\n", g_config.mem_check_interval);
    printf("  Network check interval: %d seconds\n", g_config.net_check_interval);
    printf("  Log file: %s (%s)\n", g_config.log_file, g_config.log_enabled ? "enabled" : "disabled");
    printf("\n");

    // Initialize SUSI watchdog
    if (init_susi_watchdog() != 0) {
        log_message("Failed to initialize SUSI watchdog");
        return 1;
    }

    // Initialize real-time monitoring system
    if (init_realtime_monitoring() != 0) {
        log_message("Failed to initialize real-time monitoring system");
        cleanup_susi_watchdog();
        return 1;
    }

    // Log startup information with timestamp
    char startup_msg[256];
    snprintf(startup_msg, sizeof(startup_msg),
             "System watchdog monitor started (timeout: %d sec, real-time monitoring)",
             g_config.watchdog_timeout);
    log_message(startup_msg);

    // Main monitoring loop
    printf("Starting real-time system monitoring...\n");
    log_message("Real-time system monitoring started");

    time_t last_grace_feed = 0;

    while (g_running) {
        struct epoll_event events[MAX_EPOLL_EVENTS];
        time_t now = time(NULL);

        // Calculate timeout for epoll_wait (quarter watchdog timeout for responsiveness)
        int timeout = (g_config.watchdog_timeout * 1000) / 4;

        int num_events = epoll_wait(g_epoll_fd, events, MAX_EPOLL_EVENTS, timeout);

        if (num_events == -1) {
            if (errno == EINTR) {
                continue;  // Interrupted by signal, continue
            }
            perror("epoll_wait failed");
            break;
        }

        // Process timer events
        for (int i = 0; i < num_events; i++) {
            int fd = events[i].data.fd;

            if (fd == g_cpu_timer_fd || fd == g_mem_timer_fd || fd == g_net_timer_fd) {
                // Handle timer event (read to clear the timer)
                handle_timer_event(fd);

                // Check system activity for this specific resource
                int activity = check_system_activity(fd);

                if (activity == -1) {
                    // Critical condition detected (e.g., max CPU threshold exceeded)
                    log_message("Critical system condition detected - stopping watchdog to trigger reboot");
                    g_running = 0;
                    break;
                } else if (activity == -2) {
                    // Watchdog feed error
                    log_message("Watchdog feed error - stopping monitoring");
                    g_running = 0;
                    break;
                }
            }
        }

        // Handle timeout case - check for grace period feeding
        if (num_events == 0) {
            // No timer events occurred within timeout period
            now = time(NULL);

            // If no recent activity, increment inactive cycles
            if (!g_activity_detected) {
                // Only increment if enough time has passed since last check
                static time_t last_inactive_check = 0;
                if (now - last_inactive_check >= g_config.watchdog_timeout / 4) {
                    g_inactive_cycles++;
                    last_inactive_check = now;

                    // Check if we should still feed watchdog during grace period
                    if (g_inactive_cycles <= g_config.max_inactive_cycles) {
                        // Grace period - continue feeding watchdog
                        if (now - last_grace_feed >= g_config.watchdog_timeout / 2) {
                            if (feed_watchdog() == 0) {
                                g_watchdog_feeds++;
                                last_grace_feed = now;

                                char log_buffer[256];
                                snprintf(log_buffer, sizeof(log_buffer),
                                       "Grace period feed #%lu (inactive cycle %d/%d)",
                                       g_watchdog_feeds, g_inactive_cycles, g_config.max_inactive_cycles);
                                log_message(log_buffer);
                            } else {
                                log_message("Failed to feed watchdog during grace period");
                                g_running = 0;
                                break;
                            }
                        }
                    } else {
                        // Exceeded grace period - stop feeding watchdog
                        log_message("Maximum inactive cycles exceeded - stopping watchdog to trigger reboot");
                        g_running = 0;
                        break;
                    }
                }
            } else {
                // Reset activity flag after timeout check
                g_activity_detected = 0;
            }
        }
    }

    // Cleanup
    if (g_running) {
        log_message("Shutting down normally");
        cleanup_susi_watchdog();
    } else {
        log_message("System will reboot via watchdog timeout");
        // Don't cleanup watchdog - let it reboot the system
    }
    
    // Cleanup real-time monitoring
    cleanup_realtime_monitoring();
    
    // Close log file
    close_log_file();
    
    return 0;
} 