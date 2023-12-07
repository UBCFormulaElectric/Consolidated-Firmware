#include <stdbool.h>
#include "io_efuse.h"

typedef enum
{
    POWER_MANAGER_SHUTDOWN,
    POWER_MANAGER_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

/**
 * Sets the Power Manager state
 * @param state to open/close
 */
void app_powerManager_setState(PowerManagerState state);
