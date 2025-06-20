#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "Susi4.h"

// Function prototypes
bool initializeSUSI(void);
void displayWatchdogInfo(SusiId_t id);
bool startWatchdog(SusiId_t id, uint32_t delayTime, uint32_t eventTime, uint32_t resetTime, uint32_t eventType);
bool triggerWatchdog(SusiId_t id);
bool stopWatchdog(SusiId_t id);
void displayMenu(void);
int getch(void);
void clearScreen(void);
void cleanupSUSI(void);

int main(int argc, char* argv[]) {
    int choice;
    SusiId_t watchdogId = SUSI_ID_WATCHDOG_1;
    uint32_t delayTime = 10000;  // 10 seconds initial delay
    uint32_t eventTime = 5000;   // 5 seconds event timeout
    uint32_t resetTime = 1000;   // 1 second reset time
    uint32_t eventType = SUSI_WDT_EVENT_TYPE_NONE; // Default event type
    bool watchdogRunning = false;
    
    printf("SUSI API Watchdog Test Application\n");
    printf("==================================\n\n");
    
    // Initialize the SUSI API
    if (!initializeSUSI()) {
        printf("Failed to initialize SUSI API. Press any key to exit...\n");
        getch();
        return -1;
    }
    
    printf("SUSI API initialized successfully!\n\n");
    
    // Display information about the watchdog
    displayWatchdogInfo(watchdogId);
    
    // Main menu loop
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                // Configure watchdog parameters
                printf("\nEnter initial delay time in milliseconds: ");
                scanf("%u", &delayTime);
                
                printf("Enter event timeout in milliseconds: ");
                scanf("%u", &eventTime);
                
                printf("Enter reset timeout in milliseconds: ");
                scanf("%u", &resetTime);
                
                printf("\nSelect event type:\n");
                printf("1. None (SUSI_WDT_EVENT_TYPE_NONE)\n");
                printf("2. IRQ (SUSI_WDT_EVENT_TYPE_IRQ)\n");
                printf("3. SCI (SUSI_WDT_EVENT_TYPE_SCI)\n");
                printf("4. Power Cycle (SUSI_WDT_EVENT_TYPE_PWRCYCLE)\n");
                printf("5. PIN (SUSI_WDT_EVENT_TYPE_PIN)\n");
                printf("Enter choice (1-5): ");
                
                int eventChoice;
                scanf("%d", &eventChoice);
                
                switch(eventChoice) {
                    case 2: eventType = SUSI_WDT_EVENT_TYPE_IRQ; break;
                    case 3: eventType = SUSI_WDT_EVENT_TYPE_SCI; break;
                    case 4: eventType = SUSI_WDT_EVENT_TYPE_PWRBTN; break;
                    case 5: eventType = SUSI_WDT_EVENT_TYPE_PIN; break;
                    default: eventType = SUSI_WDT_EVENT_TYPE_NONE; break;
                }
                
                printf("\nWatchdog configuration updated.\n");
                printf("Initial delay: %u ms, Event timeout: %u ms, Reset timeout: %u ms\n", 
                       delayTime, eventTime, resetTime);
                break;
                
            case 2:
                // Start the watchdog
                if (watchdogRunning) {
                    printf("\nWatchdog is already running. Stop it first.\n");
                } else {
                    if (startWatchdog(watchdogId, delayTime, eventTime, resetTime, eventType)) {
                        watchdogRunning = true;
                        printf("\nWatchdog started successfully!\n");
                        printf("WARNING: System behavior:\n");
                        printf("  - After %u ms initial delay + %u ms event timeout (%u ms total): Event will trigger\n", 
                               delayTime, eventTime, delayTime + eventTime);
                        printf("  - After additional %u ms reset timeout: System will restart\n", resetTime);
                        printf("  - Total time before restart: %u ms\n", delayTime + eventTime + resetTime);
                    } else {
                        printf("\nFailed to start watchdog.\n");
                    }
                }
                break;
                
            case 3:
                // Trigger (feed) the watchdog
                if (!watchdogRunning) {
                    printf("\nWatchdog is not running. Start it first.\n");
                } else {
                    if (triggerWatchdog(watchdogId)) {
                        printf("\nWatchdog triggered (reset timer).\n");
                        printf("System will restart in %u ms unless triggered again.\n", delayTime);
                    } else {
                        printf("\nFailed to trigger watchdog.\n");
                    }
                }
                break;
                
            case 4:
                // Stop the watchdog
                if (!watchdogRunning) {
                    printf("\nWatchdog is not running.\n");
                } else {
                    if (stopWatchdog(watchdogId)) {
                        watchdogRunning = false;
                        printf("\nWatchdog stopped successfully.\n");
                    } else {
                        printf("\nFailed to stop watchdog.\n");
                    }
                }
                break;
                
            case 5:
                // Let watchdog trigger (simulate hang)
                if (!watchdogRunning) {
                    printf("\nWatchdog is not running. Start it first.\n");
                } else {
                    printf("\nSimulating system hang...\n");
                    printf("System should restart in %u ms.\n", delayTime);
                    printf("Press any key to abort (if you're quick enough)...\n");
                    
                    // Wait until either a key is pressed or the system resets
                    if (getch()) {
                        printf("\nSimulation aborted.\n");
                    }
                }
                break;
                
            case 0:
                // Exit
                printf("\nExiting application...\n");
                break;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
        
        printf("\nPress any key to continue...\n");
        getch();
        
    } while (choice != 0);
    
    // Ensure watchdog is stopped before exiting
    if (watchdogRunning) {
        printf("Stopping watchdog before exit...\n");
        stopWatchdog(watchdogId);
    }
    
    // Clean up
    cleanupSUSI();
    
    return 0;
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
        printf("1. Run with sudo privileges: sudo LD_LIBRARY_PATH=./SUSI4.2.23739/Driver:$LD_LIBRARY_PATH ./watchdog_test\n");
        printf("2. Check if SUSI drivers are installed and loaded\n");
        printf("3. Verify hardware compatibility with SUSI API\n");
        printf("4. Check if this is an embedded system with SUSI support\n");
        
        return false;
    }
    
    printf("SUSI API initialized successfully!\n");
    return true;
}

// Display watchdog capabilities and information
void displayWatchdogInfo(SusiId_t id) {
    SusiStatus_t status;
    uint32_t value;
    bool supported = false;
    
    printf("Watchdog Information (ID: %d)\n", id);
    printf("---------------------------\n");
    
    // Check if watchdog is supported
    status = SusiWDogGetCaps(id, SUSI_ID_WDT_SUPPORT_FLAGS, &value);
    if (status == SUSI_STATUS_SUCCESS) {
        supported = true;
        printf("Watchdog is supported.\n");
        
        // Get time unit
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_UNIT_MINIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            printf("Time unit: %u ms\n", value);
        }
        
        // Get delay time range
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_DELAY_MINIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            printf("Minimum delay time: %u ms\n", value);
        }
        
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_DELAY_MAXIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            printf("Maximum delay time: %u ms\n", value);
        }
        
        // Get reset time range
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_RESET_MINIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            printf("Minimum reset time: %u ms\n", value);
        }
        
        status = SusiWDogGetCaps(id, SUSI_ID_WDT_RESET_MAXIMUM, &value);
        if (status == SUSI_STATUS_SUCCESS) {
            printf("Maximum reset time: %u ms\n", value);
        }
        
    } else {
        printf("Watchdog is not supported or failed to get capabilities.\n");
    }
    
    printf("\n");
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

// Display the application menu
void displayMenu(void) {
    clearScreen();
    printf("SUSI API Watchdog Test Menu\n");
    printf("=========================\n");
    printf("1. Configure watchdog parameters\n");
    printf("2. Start watchdog\n");
    printf("3. Trigger (feed) watchdog\n");
    printf("4. Stop watchdog\n");
    printf("5. Simulate hang (let watchdog trigger)\n");
    printf("0. Exit\n");
    printf("\nEnter your choice: ");
}

// Clean up SUSI API
void cleanupSUSI(void) {
    SusiLibUninitialize();
    printf("SUSI API cleaned up.\n");
}

// Linux replacement for getch() function
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

// Clear screen for Linux terminals
void clearScreen(void) {
    printf("\033[2J\033[H");
}
