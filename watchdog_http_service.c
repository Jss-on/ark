#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <microhttpd.h>
#include <jansson.h>
#include "Susi4.h"

// Configuration
#define DEFAULT_PORT 9101  // Aligned with Prometheus exporter port range
#define DEFAULT_DELAY_TIME 10000   // 10 seconds initial delay
#define DEFAULT_EVENT_TIME 5000    // 5 seconds event timeout
#define DEFAULT_RESET_TIME 1000    // 1 second reset time
#define DEFAULT_EVENT_TYPE SUSI_WDT_EVENT_TYPE_NONE

// Global variables
static struct MHD_Daemon *http_daemon = NULL;
static SusiId_t watchdogId = SUSI_ID_WATCHDOG_1;
static uint32_t delayTime = DEFAULT_DELAY_TIME;
static uint32_t eventTime = DEFAULT_EVENT_TIME;
static uint32_t resetTime = DEFAULT_RESET_TIME;
static uint32_t eventType = DEFAULT_EVENT_TYPE;
static bool watchdogRunning = false;
static bool susiInitialized = false;

// Function prototypes
bool initializeSUSI(void);
void cleanupSUSI(void);
bool startWatchdog(SusiId_t id, uint32_t delayTime, uint32_t eventTime, uint32_t resetTime, uint32_t eventType);
bool triggerWatchdog(SusiId_t id);
bool stopWatchdog(SusiId_t id);
json_t* getWatchdogInfo(SusiId_t id);
json_t* getWatchdogStatus(void);

// Signal handling for graceful shutdown
static volatile int keepRunning = 1;

void sigHandler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        printf("Shutdown signal received. Cleaning up...\n");
        keepRunning = 0;
    }
}

// HTTP request handler
static enum MHD_Result requestHandler(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    
    struct MHD_Response *response;
    int ret;
    json_t *json_response = NULL;
    char *json_str = NULL;
    
    // For parsing form data
    static int dummy;
    const char *param_value;
    
    // Prevent unused parameter warnings
    (void)cls;
    (void)version;
    (void)upload_data;
    
    // Return a dummy value on first call (MHD requires this pattern)
    if (*con_cls == NULL) {
        *con_cls = &dummy;
        return MHD_YES;
    }
    
    // We don't accept upload data right now
    if (*upload_data_size != 0) {
        *upload_data_size = 0;
        return MHD_YES;
    }
    
    // Route requests based on URL and method
    printf("Received request: %s %s\n", method, url);
    
    // API endpoints
    if (strcmp(method, "GET") == 0) {
        // GET /api/status - Get current watchdog status
        if (strcmp(url, "/api/status") == 0) {
            json_response = getWatchdogStatus();
        }
        // GET /api/info - Get watchdog capabilities
        else if (strcmp(url, "/api/info") == 0) {
            json_response = getWatchdogInfo(watchdogId);
        }
        // GET / - Root endpoint (simple status page)
        else if (strcmp(url, "/") == 0 || strcmp(url, "/index.html") == 0) {
            // Return a simple HTML status page
            char html_buffer[2048]; // Use fixed buffer with known size instead of const char* to avoid strcat issues
            
            // Build the HTML in parts to avoid buffer overflow
            snprintf(html_buffer, sizeof(html_buffer),
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "    <title>Watchdog HTTP Service</title>"
                "    <style>"
                "        body { font-family: Arial, sans-serif; margin: 40px; line-height: 1.6; }"
                "        h1 { color: #333; }"
                "        .status { display: inline-block; padding: 5px 10px; border-radius: 4px; }"
                "        .running { background-color: #d4edda; color: #155724; }"
                "        .stopped { background-color: #f8d7da; color: #721c24; }"
                "        .endpoints { background-color: #f8f9fa; padding: 15px; border-radius: 4px; }"
                "        pre { background-color: #f1f1f1; padding: 10px; border-radius: 4px; }"
                "    </style>"
                "</head>"
                "<body>"
                "    <h1>Watchdog HTTP Service</h1>"
                "    <p>Status: <span class='status %s'>%s</span></p>"
                "    <h2>Available Endpoints</h2>"
                "    <div class='endpoints'>"
                "        <h3>Status</h3>"
                "        <p>GET /api/status - Current watchdog status (JSON)</p>"
                "        <p>GET /api/info - Watchdog capabilities (JSON)</p>"
                ""
                "        <h3>Control</h3>"
                "        <p>POST /api/start - Start the watchdog</p>"
                "        <p>POST /api/trigger - Feed/trigger the watchdog</p>"
                "        <p>POST /api/stop - Stop the watchdog</p>"
                "        <p>POST /api/configure - Configure watchdog parameters</p>"
                "    </div>"
                ""
                "    <h2>Example Usage</h2>"
                "    <pre>curl http://localhost:9101/api/status</pre>"
                "    <pre>curl -X POST http://localhost:9101/api/start</pre>"
                "    <pre>curl -X POST \"http://localhost:9101/api/configure?delay=15000&event=5000&reset=1000&type=0\"</pre>"
                "</body>"
                "</html>",
                watchdogRunning ? "running" : "stopped",
                watchdogRunning ? "Running" : "Stopped"
            );
            
            // Create response from the HTML buffer
            response = MHD_create_response_from_buffer(strlen(html_buffer),
                                                     (void *)html_buffer,
                                                     MHD_RESPMEM_MUST_COPY);
            if (response == NULL) {
                return MHD_NO;
            }
            
            MHD_add_response_header(response, "Content-Type", "text/html");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        else {
            // Unknown GET endpoint
            json_response = json_pack("{ss}", "error", "Unknown endpoint");
            ret = MHD_HTTP_NOT_FOUND;
        }
    }
    else if (strcmp(method, "POST") == 0) {
        // POST /api/start - Start the watchdog
        if (strcmp(url, "/api/start") == 0) {
            if (watchdogRunning) {
                json_response = json_pack("{ss}", "error", "Watchdog is already running");
            } else {
                // Check for custom parameters
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "delay");
                if (param_value) delayTime = atoi(param_value);
                
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "event");
                if (param_value) eventTime = atoi(param_value);
                
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "reset");
                if (param_value) resetTime = atoi(param_value);
                
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "type");
                if (param_value) eventType = atoi(param_value);
                
                if (startWatchdog(watchdogId, delayTime, eventTime, resetTime, eventType)) {
                    watchdogRunning = true;
                    json_response = json_pack("{sssisisisi}",
                                            "status", "Watchdog started",
                                            "delay", delayTime,
                                            "event", eventTime,
                                            "reset", resetTime,
                                            "type", eventType);
                } else {
                    json_response = json_pack("{ss}", "error", "Failed to start watchdog");
                }
            }
        }
        // POST /api/trigger - Feed/trigger the watchdog
        else if (strcmp(url, "/api/trigger") == 0) {
            if (!watchdogRunning) {
                json_response = json_pack("{ss}", "error", "Watchdog is not running");
            } else {
                if (triggerWatchdog(watchdogId)) {
                    json_response = json_pack("{ss}", "status", "Watchdog triggered (reset timer)");
                } else {
                    json_response = json_pack("{ss}", "error", "Failed to trigger watchdog");
                }
            }
        }
        // POST /api/stop - Stop the watchdog
        else if (strcmp(url, "/api/stop") == 0) {
            if (!watchdogRunning) {
                json_response = json_pack("{ss}", "error", "Watchdog is not running");
            } else {
                if (stopWatchdog(watchdogId)) {
                    watchdogRunning = false;
                    json_response = json_pack("{ss}", "status", "Watchdog stopped");
                } else {
                    json_response = json_pack("{ss}", "error", "Failed to stop watchdog");
                }
            }
        }
        // POST /api/configure - Configure watchdog parameters (when stopped)
        else if (strcmp(url, "/api/configure") == 0) {
            if (watchdogRunning) {
                json_response = json_pack("{ss}", "error", "Cannot configure watchdog while running. Stop it first.");
            } else {
                // Get parameters from URL query
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "delay");
                if (param_value) delayTime = atoi(param_value);
                
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "event");
                if (param_value) eventTime = atoi(param_value);
                
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "reset");
                if (param_value) resetTime = atoi(param_value);
                
                param_value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "type");
                if (param_value) eventType = atoi(param_value);
                
                json_response = json_pack("{sssisisisi}",
                                        "status", "Watchdog configured",
                                        "delay", delayTime,
                                        "event", eventTime,
                                        "reset", resetTime,
                                        "type", eventType);
            }
        }
        else {
            // Unknown POST endpoint
            json_response = json_pack("{ss}", "error", "Unknown endpoint");
            ret = MHD_HTTP_NOT_FOUND;
        }
    }
    else {
        // Method not allowed
        json_response = json_pack("{ss}", "error", "Method not allowed");
        ret = MHD_HTTP_METHOD_NOT_ALLOWED;
    }
    
    // If we have a JSON response, send it
    if (json_response) {
        json_str = json_dumps(json_response, JSON_COMPACT);
        if (json_str) {
            response = MHD_create_response_from_buffer(strlen(json_str),
                                                     (void*)json_str,
                                                     MHD_RESPMEM_MUST_FREE);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            json_decref(json_response);
            return ret;
        }
        json_decref(json_response);
    }
    
    // If we get here, something went wrong with the JSON processing
    const char *error_msg = "Internal server error";
    response = MHD_create_response_from_buffer(strlen(error_msg),
                                             (void*)error_msg,
                                             MHD_RESPMEM_MUST_COPY);
    ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
    MHD_destroy_response(response);
    return ret;
}

// Get current watchdog status as JSON
json_t* getWatchdogStatus(void) {
    json_t *json_status = json_object();
    
    json_object_set_new(json_status, "running", json_boolean(watchdogRunning));
    json_object_set_new(json_status, "susi_initialized", json_boolean(susiInitialized));
    json_object_set_new(json_status, "delay_time", json_integer(delayTime));
    json_object_set_new(json_status, "event_time", json_integer(eventTime));
    json_object_set_new(json_status, "reset_time", json_integer(resetTime));
    json_object_set_new(json_status, "event_type", json_integer(eventType));
    
    // Calculate remaining times if running
    if (watchdogRunning) {
        // Note: In a real implementation, we'd track the time since last trigger
        // For now, we just show the configured times
        json_object_set_new(json_status, "max_total_time_ms", json_integer(delayTime + eventTime + resetTime));
    }
    
    return json_status;
}

// Get watchdog capabilities and information as JSON
json_t* getWatchdogInfo(SusiId_t id) {
    SusiStatus_t status;
    uint32_t value;
    json_t *json_info = json_object();
    
    json_object_set_new(json_info, "watchdog_id", json_integer(id));
    
    // Check if watchdog is supported
    status = SusiWDogGetCaps(id, SUSI_ID_WDT_SUPPORT_FLAGS, &value);
    if (status == SUSI_STATUS_SUCCESS) {
        json_object_set_new(json_info, "supported", json_true());
        
        // Get time unit
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_UNIT_MINIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            json_object_set_new(json_info, "time_unit_ms", json_integer(value));
        }
        
        // Get delay time range
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_DELAY_MINIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            json_object_set_new(json_info, "min_delay_time_ms", json_integer(value));
        }
        
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_DELAY_MAXIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            json_object_set_new(json_info, "max_delay_time_ms", json_integer(value));
        }
        
        // Get reset time range
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_RESET_MINIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            json_object_set_new(json_info, "min_reset_time_ms", json_integer(value));
        }
        
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_RESET_MAXIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            json_object_set_new(json_info, "max_reset_time_ms", json_integer(value));
        }
        
    } else {
        json_object_set_new(json_info, "supported", json_false());
        json_object_set_new(json_info, "error", json_string("Watchdog is not supported or failed to get capabilities"));
    }
    
    return json_info;
}

// Initialize the SUSI API
bool initializeSUSI(void) {
    SusiStatus_t status = SusiLibInitialize();
    
    if (status != SUSI_STATUS_SUCCESS) {
        printf("SUSI API initialization failed with status: 0x%08X\n", status);
        
        switch (status) {
            case SUSI_STATUS_ERROR:
                printf("Error: General SUSI error\n");
                break;
            case SUSI_STATUS_NOT_FOUND:
                printf("Error: SUSI device or resource not found\n");
                break;
            case SUSI_STATUS_UNSUPPORTED:
                printf("Error: SUSI not supported on this platform\n");
                break;
            case SUSI_STATUS_INVALID_PARAMETER:
                printf("Error: Invalid parameter passed to SUSI\n");
                break;
            case SUSI_STATUS_INVALID_BLOCK_ALIGNMENT:
                printf("Error: Invalid block alignment\n");
                break;
            case SUSI_STATUS_INVALID_BLOCK_LENGTH:
                printf("Error: Invalid block length\n");
                break;
            case SUSI_STATUS_INVALID_DIRECTION:
                printf("Error: Invalid direction\n");
                break;
            case SUSI_STATUS_TIMEOUT:
                printf("Error: SUSI operation timeout\n");
                break;
            case SUSI_STATUS_RUNNING:
                printf("Error: SUSI already running\n");
                break;
            default:
                printf("Error: Unknown SUSI error code\n");
                break;
        }
        
        printf("\nPossible solutions:\n");
        printf("1. Run with sudo privileges: sudo LD_LIBRARY_PATH=./SUSI4.2.23739/Driver:$LD_LIBRARY_PATH ./watchdog_http_service\n");
        printf("2. Check if SUSI drivers are installed and loaded\n");
        printf("3. Verify hardware compatibility with SUSI API\n");
        printf("4. Check if this is an embedded system with SUSI support\n");
        
        return false;
    }
    
    printf("SUSI API initialized successfully!\n");
    susiInitialized = true;
    return true;
}

// Start the watchdog
bool startWatchdog(SusiId_t id, uint32_t delayTime, uint32_t eventTime, uint32_t resetTime, uint32_t eventType) {
    SusiStatus_t status = SusiWDogStart(id, delayTime, eventTime, resetTime, eventType);
    return (status == SUSI_STATUS_SUCCESS);
}

// Trigger (feed) the watchdog
bool triggerWatchdog(SusiId_t id) {
    SusiStatus_t status = SusiWDogTrigger(id);
    return (status == SUSI_STATUS_SUCCESS);
}

// Stop the watchdog
bool stopWatchdog(SusiId_t id) {
    SusiStatus_t status = SusiWDogStop(id);
    return (status == SUSI_STATUS_SUCCESS);
}

// Clean up SUSI API
void cleanupSUSI(void) {
    SusiLibUninitialize();
    printf("SUSI API cleaned up.\n");
}

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--port") == 0 || strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                port = atoi(argv[i + 1]);
                if (port <= 0 || port > 65535) {
                    port = DEFAULT_PORT;
                }
                i++; // Skip the port number in the next iteration
            }
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Watchdog HTTP Service\n");
            printf("Usage: %s [options]\n", argv[0]);
            printf("Options:\n");
            printf("  --port, -p PORT    Specify the HTTP server port (default: %d)\n", DEFAULT_PORT);
            printf("  --help, -h         Show this help message\n");
            return 0;
        }
    }
    
    // Set up signal handling for graceful shutdown
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    
    printf("Starting Watchdog HTTP Service...\n");
    
    // Initialize SUSI API
    susiInitialized = initializeSUSI();
    if (!susiInitialized) {
        printf("Failed to initialize SUSI API. Exiting.\n");
        return -1;
    }
    
    // Start HTTP server
    http_daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
                                   port,
                                   NULL, NULL,
                                   &requestHandler, NULL,
                                   MHD_OPTION_END);
    
    if (http_daemon == NULL) {
        printf("Failed to start HTTP server on port %d\n", port);
        cleanupSUSI();
        return -1;
    }
    
    printf("Watchdog HTTP Service running on port %d\n", port);
    printf("API endpoints available at:\n");
    printf("  GET  /api/status    - Get current watchdog status\n");
    printf("  GET  /api/info      - Get watchdog capabilities\n");
    printf("  POST /api/start     - Start the watchdog\n");
    printf("  POST /api/trigger   - Feed/trigger the watchdog\n");
    printf("  POST /api/stop      - Stop the watchdog\n");
    printf("  POST /api/configure - Configure watchdog parameters\n");
    printf("Press Ctrl+C to stop the server\n");
    
    // Main loop
    while (keepRunning) {
        sleep(1);
    }
    
    // Clean up
    printf("Stopping HTTP server...\n");
    MHD_stop_daemon(http_daemon);
    
    // Stop watchdog if running
    if (watchdogRunning) {
        printf("Stopping watchdog...\n");
        stopWatchdog(watchdogId);
    }
    
    // Clean up SUSI API
    cleanupSUSI();
    
    printf("Watchdog HTTP Service stopped.\n");
    return 0;
}
