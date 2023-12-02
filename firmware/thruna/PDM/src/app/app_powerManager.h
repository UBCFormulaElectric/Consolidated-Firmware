#include <stdbool.h>
#include "io_efuse.h"

typedef enum
{
    POWER_MANAGER_CONTRACTOR_SHUTDOWN,
    POWER_MANAGER_CONTRACTOR_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

/**
 * Initalizes to the shutdown state (closed)
 */
void app_powerManager_init();

/**
 * Sets the Power Manager state
 * @param state to open/close
 */
void app_powerManager_setState(PowerManagerState state);

/**
 * Changes the Power Manager based on the state of the BMS
 */
void app_powerManager_transition();
