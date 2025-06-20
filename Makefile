# Makefile for System Watchdog Monitor
# Adjust paths as needed for your SUSI installation

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_GNU_SOURCE
# Alternative library names to try if SUSI-4.00 is not found
# Uncomment one of these if the default doesn't work
#LDFLAGS = -lSUSI-3.02 -lm -lpthread
#LDFLAGS = -lSUSI -lm -lpthread
LDFLAGS = -lSUSI-4.00 -lm -lpthread

# SUSI header path - adjust as needed
SUSI_INCLUDE = -I./ARK-3533_SUSI4.2.23739_Release_2023_07_27_ubuntu22.04_x64/SUSI4.2.23739/SUSIDeviceDEMO/GSENSORDEMO

# SUSI library path - adjust as needed  
SUSI_LIB_PATH = -L/usr/lib

TARGET = system_watchdog_monitor
SOURCE = system_watchdog_monitor.c

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SUSI_INCLUDE) -o $(TARGET) $(SOURCE) $(SUSI_LIB_PATH) $(LDFLAGS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)

# Create systemd service file
service:
	@echo "Creating systemd service file..."
	@echo "[Unit]" > system-watchdog-monitor.service
	@echo "Description=System Watchdog Monitor" >> system-watchdog-monitor.service
	@echo "After=network.target" >> system-watchdog-monitor.service
	@echo "" >> system-watchdog-monitor.service
	@echo "[Service]" >> system-watchdog-monitor.service
	@echo "Type=simple" >> system-watchdog-monitor.service
	@echo "ExecStart=/usr/local/bin/system_watchdog_monitor" >> system-watchdog-monitor.service
	@echo "Restart=always" >> system-watchdog-monitor.service
	@echo "RestartSec=10" >> system-watchdog-monitor.service
	@echo "User=root" >> system-watchdog-monitor.service
	@echo "" >> system-watchdog-monitor.service
	@echo "[Install]" >> system-watchdog-monitor.service
	@echo "WantedBy=multi-user.target" >> system-watchdog-monitor.service
	@echo "Service file created: system-watchdog-monitor.service"

# Install systemd service
install-service: service install
	sudo cp system-watchdog-monitor.service /etc/systemd/system/
	sudo systemctl daemon-reload
	sudo systemctl enable system-watchdog-monitor.service
	@echo "Service installed and enabled. Start with: sudo systemctl start system-watchdog-monitor"

# Uninstall systemd service
uninstall-service:
	sudo systemctl stop system-watchdog-monitor.service || true
	sudo systemctl disable system-watchdog-monitor.service || true
	sudo rm -f /etc/systemd/system/system-watchdog-monitor.service
	sudo systemctl daemon-reload

help:
	@echo "Available targets:"
	@echo "  all              - Build the program"
	@echo "  clean            - Remove built files"
	@echo "  install          - Install to /usr/local/bin"
	@echo "  service          - Create systemd service file"
	@echo "  install-service  - Install as systemd service"
	@echo "  uninstall-service- Remove systemd service"
	@echo "  help             - Show this help"
