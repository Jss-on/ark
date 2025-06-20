# System Watchdog Monitor: Guide

## Features

The System Watchdog Monitor is a reliability solution that:
- Monitors system health through CPU, memory, and network activity
- Uses hardware watchdog timer to automatically reboot frozen systems
- Provides detailed logging of system activity and watchdog events
- Can be configured to your specific monitoring needs
- Runs as a system service for automatic startup on boot

## Configuration Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| watchdog_timeout | Time in seconds before automatic reboot if system freezes | 60 |
| monitor_interval | How often the system activity is checked (seconds) | 10 |
| max_inactive_cycles | Number of inactive checks before triggering reboot | 3 |
| cpu_threshold | CPU usage % considered as activity | 5.0 |
| mem_threshold | Memory change in bytes considered as activity | 1024 |
| net_threshold | Network traffic in bytes considered as activity | 100 |
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
```
Run systemctl restart for these changes to reflect
```bash
sudo systemctl restart system-watchdog-monitor
```

## Monitoring

### View Log File
```bash
sudo tail -f /var/log/system_watchdog_monitor.log
```

### Understanding Log Messages
- **[Timestamp] System activity detected - watchdog fed #N [CPU:active MEM:active NET:idle]**  
  Shows which resources triggered watchdog feed (CPU, memory, network)
  
- **[Timestamp] Watchdog fed #N - system healthy**  
  Periodic summary of ongoing watchdog operation
  
- **[Timestamp] No system activity detected (cycle X/Y)**  
  Warning that system may be inactive; will reboot if it reaches maximum cycles

### Service Logs
```bash
sudo journalctl -u system-watchdog-monitor
```

## Troubleshooting

If the system is rebooting unexpectedly:
1. Check logs for missed watchdog feeds
2. Consider increasing thresholds if normal activity isn't being detected
3. Increase max_inactive_cycles for more tolerance before reboot

If the service won't start:
1. Verify SUSI drivers are installed
2. Check for permissions issues (service must run as root)
3. Review logs for specific errors

