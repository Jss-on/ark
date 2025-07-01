# System Watchdog Monitor: Real-Time Event-Driven Guide

## Features

The System Watchdog Monitor is a real-time reliability solution that:
- **Real-time monitoring** using Linux epoll and timerfd for immediate activity detection
- **Event-driven architecture** eliminates polling delays for instant response
- **Independent resource monitoring** with separate timers for CPU, memory, and network
- **Hardware watchdog integration** via SUSI API for automatic system recovery
- **Configurable thresholds** and monitoring intervals for each resource type
- **Grace period management** to prevent false-positive reboots
- **Comprehensive logging** with timestamped activity tracking
- **Signal handling** for graceful shutdown and cleanup

## Configuration Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| watchdog_timeout | Hardware watchdog timeout (seconds) before system reboot | 60 |
| max_inactive_cycles | Grace period cycles before triggering reboot | 3 |
| cpu_threshold | CPU usage % threshold for activity detection | 5.0 |
| max_cpu_threshold | Critical CPU usage % that triggers immediate reboot | 90.0 |
| mem_threshold | Memory change (bytes) threshold for activity detection | 1024 |
| net_threshold | Network traffic (bytes) threshold for activity detection | 100 |
| cpu_check_interval | CPU monitoring timer interval (seconds) | 1 |
| mem_check_interval | Memory monitoring timer interval (seconds) | 2 |
| net_check_interval | Network monitoring timer interval (seconds) | 1 |
| log_file | Path to the log file | /var/log/system_watchdog_monitor.log |
| log_enabled | Enable (1) or disable (0) file logging | 1 |

## How to Change Configuration

### Method 1: Edit Configuration File
```bash
sudo nano /etc/system_watchdog_monitor.conf
```
Add or modify the parameters you want to change:
```
watchdog_timeout=90
cpu_threshold=2.0
cpu_check_interval=2
mem_check_interval=3
net_check_interval=1
max_cpu_threshold=85.0
```
Restart the service for changes to take effect:
```bash
sudo systemctl restart system-watchdog-monitor
```

### Method 2: Command Line Arguments
```bash
./system_watchdog_monitor --watchdog-timeout 90 --cpu-threshold 2.0 --cpu-interval 2
```

### Available Command Line Options
```
-w, --watchdog-timeout SEC    Set watchdog timeout (default: 60)
-i, --max-inactive-cycles N   Set max inactive cycles (default: 3)
-p, --cpu-threshold PERCENT   Set CPU activity threshold (default: 5.0)
-x, --max-cpu-threshold PCT   Set max CPU threshold (default: 90.0)
-e, --mem-threshold BYTES     Set memory activity threshold (default: 1024)
-n, --net-threshold BYTES     Set network activity threshold (default: 100)
    --cpu-interval SEC        Set CPU check interval (default: 1)
    --mem-interval SEC        Set memory check interval (default: 2)
    --net-interval SEC        Set network check interval (default: 1)
-l, --log-file FILE          Set log file path
-d, --disable-log            Disable file logging
-c, --config FILE            Load configuration from file
-h, --help                   Display help message
```

## Monitoring

### View Log File
```bash
sudo tail -f /var/log/system_watchdog_monitor.log
```

### Understanding Log Messages

#### Real-Time Activity Detection
- **[Timestamp] Activity detected - watchdog fed #N [CPU:active MEM:idle NET:active]**  
  Real-time detection showing which resources triggered immediate watchdog feeding
  
- **[Timestamp] CPU activity: 15.2% (threshold: 5.0%, max: 90.0%)**  
  Detailed CPU usage when activity threshold is exceeded
  
- **[Timestamp] Memory activity: 2048 bytes change (threshold: 1024)**  
  Memory usage changes that triggered activity detection
  
- **[Timestamp] Network activity: RX:1500 TX:800 bytes (threshold: 100)**  
  Network traffic that exceeded the activity threshold

#### System Health Status
- **[Timestamp] Watchdog fed #N - system healthy**  
  Periodic confirmation of successful watchdog feeding
  
- **[Timestamp] Real-time system monitoring started**  
  Service initialization with event-driven monitoring active

#### Grace Period and Critical Conditions
- **[Timestamp] Grace period feed #N (inactive cycle X/Y)**  
  Watchdog feeding during grace period when no activity detected
  
- **[Timestamp] CRITICAL: CPU usage 95.5% exceeds maximum threshold 90.0%!**  
  Critical condition that triggers immediate watchdog stop and reboot
  
- **[Timestamp] Maximum inactive cycles exceeded - stopping watchdog to trigger reboot**  
  Final warning before allowing system reboot due to prolonged inactivity

### Service Logs
```bash
# View real-time service logs
sudo journalctl -u system-watchdog-monitor -f

# View recent service logs with timestamps
sudo journalctl -u system-watchdog-monitor --since "1 hour ago"

# View logs with specific priority
sudo journalctl -u system-watchdog-monitor -p err
```

## Troubleshooting

### System Rebooting Unexpectedly
1. **Check real-time activity detection**:
   ```bash
   sudo tail -f /var/log/system_watchdog_monitor.log | grep "activity"
   ```
2. **Adjust activity thresholds** if normal system activity isn't being detected:
   - Lower `cpu_threshold` (e.g., from 5.0% to 2.0%)
   - Lower `mem_threshold` (e.g., from 1024 to 512 bytes)
   - Lower `net_threshold` (e.g., from 100 to 50 bytes)
3. **Increase grace period** for more tolerance:
   - Increase `max_inactive_cycles` (e.g., from 3 to 5)
4. **Adjust monitoring intervals** for different responsiveness:
   - Increase intervals for less sensitive monitoring
   - Decrease intervals for more responsive detection

### Service Won't Start
1. **Verify SUSI drivers and hardware support**:
   ```bash
   lsmod | grep susi
   dmesg | grep -i watchdog
   ```
2. **Check permissions** (service must run as root):
   ```bash
   sudo systemctl status system-watchdog-monitor
   ```
3. **Review detailed logs**:
   ```bash
   sudo journalctl -u system-watchdog-monitor -f
   ```
4. **Test manual execution**:
   ```bash
   sudo ./system_watchdog_monitor --help
   sudo ./system_watchdog_monitor -d  # Run without log file
   ```

### Performance Tuning
- **High CPU systems**: Increase `cpu_check_interval` to reduce monitoring overhead
- **Network-intensive systems**: Adjust `net_threshold` to avoid false activity detection
- **Memory-constrained systems**: Fine-tune `mem_threshold` based on typical memory patterns
- **Critical systems**: Lower `max_inactive_cycles` for faster recovery

### Real-Time Monitoring Verification
To verify the event-driven system is working:
```bash
# Monitor timer events in real-time
sudo strace -e epoll_wait,read -p $(pgrep system_watchdog_monitor)

# Check timer file descriptors
sudo ls -la /proc/$(pgrep system_watchdog_monitor)/fd/
```

