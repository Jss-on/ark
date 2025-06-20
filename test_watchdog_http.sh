#!/bin/bash
# Test script for watchdog HTTP service
# This script demonstrates how to interact with the watchdog service API

# Configuration
HOST="localhost"
PORT="8080"
BASE_URL="http://$HOST:$PORT"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Helper functions
function print_header() {
  echo -e "${BLUE}==== $1 ====${NC}"
}

function print_success() {
  echo -e "${GREEN}SUCCESS: $1${NC}"
}

function print_error() {
  echo -e "${RED}ERROR: $1${NC}"
}

function print_info() {
  echo -e "${YELLOW}INFO: $1${NC}"
}

function check_service() {
  print_header "Checking if service is running"
  if curl -s "$BASE_URL/api/status" > /dev/null; then
    print_success "Service is running"
    return 0
  else
    print_error "Service is not running on $BASE_URL"
    return 1
  fi
}

# Check if service is running
if ! check_service; then
  print_info "Make sure the watchdog HTTP service is running with:"
  print_info "  make -f Makefile.watchdog_http run-sudo"
  exit 1
fi

# Display API information
print_header "Retrieving watchdog capabilities"
curl -s "$BASE_URL/api/info" | jq || echo "Response: $(curl -s "$BASE_URL/api/info")"

# Display current status
print_header "Retrieving current watchdog status"
curl -s "$BASE_URL/api/status" | jq || echo "Response: $(curl -s "$BASE_URL/api/status")"

# Configure watchdog
print_header "Configuring watchdog parameters"
curl -s -X POST "$BASE_URL/api/configure?delay=20000&event=5000&reset=1000&type=0" | jq || echo "Response: $(curl -s -X POST "$BASE_URL/api/configure?delay=20000&event=5000&reset=1000&type=0")"

# Start watchdog
print_header "Starting watchdog"
curl -s -X POST "$BASE_URL/api/start" | jq || echo "Response: $(curl -s -X POST "$BASE_URL/api/start")"

# Check status again
print_header "Checking watchdog status after starting"
curl -s "$BASE_URL/api/status" | jq || echo "Response: $(curl -s "$BASE_URL/api/status")"

# Feed watchdog
print_header "Feeding watchdog"
curl -s -X POST "$BASE_URL/api/trigger" | jq || echo "Response: $(curl -s -X POST "$BASE_URL/api/trigger")"

# Sleep for a moment
print_info "Waiting for 5 seconds..."
sleep 5

# Feed watchdog again
print_header "Feeding watchdog again"
curl -s -X POST "$BASE_URL/api/trigger" | jq || echo "Response: $(curl -s -X POST "$BASE_URL/api/trigger")"

# Stop watchdog
print_header "Stopping watchdog"
curl -s -X POST "$BASE_URL/api/stop" | jq || echo "Response: $(curl -s -X POST "$BASE_URL/api/stop")"

print_header "Test complete"
print_info "The watchdog has been started, fed, and then stopped"
print_info "For a real test of system reliability:"
print_info "1. Start the watchdog"
print_info "2. Configure the watchdog_controller to feed it only when system health checks pass"
print_info "3. Simulate an unhealthy system condition by running a resource-intensive task"
print_info "4. Observe the controller stop feeding the watchdog, triggering a system restart"
