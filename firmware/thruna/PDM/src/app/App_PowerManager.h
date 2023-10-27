#include <stdbool.h>
#include "Io_Efuse.h"

typedef enum
{
    POWER_MANAGER_CONTRACTOR_OPEN,
    POWER_MANAGER_CONTRACTOR_CLOSED,
    NUM_POWER_STATES
} PowerManagerState;

/**
 * Initalizes to the drive state (closed)
 */
void PowerManager_Init();

/**
 * Sets the Power Manager state
 * @param state to open/close
 */
void PowerManager_SetState(PowerManagerState state);
