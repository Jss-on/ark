#!/bin/bash
# Script to run the SUSI watchdog test application with proper environment

echo "Starting SUSI Watchdog Test Application..."
echo "Note: This requires sudo privileges for hardware access"
echo ""

# Set the library path and run with sudo
sudo LD_LIBRARY_PATH=./SUSI4.2.23739/Driver:$LD_LIBRARY_PATH ./watchdog_test
