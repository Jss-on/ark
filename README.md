# System Watchdog Monitor: Real-Time Event-Driven Guide

## Watchdog HTTP Service

This project provides an HTTP service interface to hardware watchdog functionality using the SUSI API.

## Prerequisites

- SUSI API libraries (included in `SUSI4.2.23739/` directory)
- libmicrohttpd-dev
- libjansson-dev
- build-essential

## Building

### Direct build

```bash
# Install dependencies
sudo apt-get install libmicrohttpd-dev libjansson-dev

# Build the service
make -f Makefile.watchdog_http
```

## Running

### Running directly

The service requires elevated privileges to access hardware watchdog features:

```bash
# Run with sudo
sudo LD_LIBRARY_PATH=./SUSI4.2.23739/Driver:$LD_LIBRARY_PATH ./watchdog_http_service

# Or use the make target
make -f Makefile.watchdog_http run-sudo
```

### Running with Docker

The service can also be run in a Docker container with hardware access:

```bash
# Build and run using docker-compose
docker-compose up -d

# Check logs
docker-compose logs -f

# Stop the service
docker-compose down
```

## API Endpoints

The service runs on port 9101 and provides the following endpoints:

- `GET /api/status` - Get current watchdog status
- `GET /api/info` - Get watchdog capabilities
- `POST /api/start` - Start the watchdog
- `POST /api/trigger` - Feed/trigger the watchdog
- `POST /api/stop` - Stop the watchdog
- `POST /api/configure` - Configure watchdog parameters

## Testing

Test scripts are provided for both Bash and PowerShell:

```bash
# Bash
chmod +x test_watchdog_http.sh
./test_watchdog_http.sh

```

## Integration with Prometheus

The service is designed to work with Prometheus monitoring. It runs on port 9101, which aligns with the standard Prometheus exporter port range, making it easy to integrate with your monitoring infrastructure.

## Notes

- The watchdog will automatically restart the system if not fed within the configured timeout
- The service must be run with elevated privileges due to hardware access requirements
- When using Docker, the container must run in privileged mode
