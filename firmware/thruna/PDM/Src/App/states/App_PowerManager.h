#include <stdbool.h>
#include "Io_Efuse.h"

typedef enum
{
    POWER_MANAGER_CONTRACTOR_OPEN,
    POWER_MANAGER_CONTRACTOR_CLOSED,
    NUM_POWER_STATES
} PowerManagerState;

/**
 * Initalizes the ddrive state: LVPWW, DI_LHS, DI_RHS and AIR to closed and others to open
 */
void PowerManager_Init();

/**
 * Sets the Power Manager state
 * @param state to open/close
 */
void PowerManager_SetState(PowerManagerState state);
