#include <stdbool.h>
#include "Io_Efuse.h"

typedef enum
{
    POWER_MANAGER_CONTRACTOR_SHUTDOWN,
    POWER_MANAGER_CONTRACTOR_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

/**
 * Initalizes to the drive state (closed)
 */
void App_PowerManager_Init();

/**
 * Sets the Power Manager state
 * @param state to open/close
 */
void App_PowerManager_SetState(PowerManagerState state);
