#include <stdbool.h>
#include "io_efuse.h"

typedef enum
{
    POWER_MANAGER_SHUTDOWN,
    POWER_MANAGER_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

static PowerManagerState current_power_state = POWER_MANAGER_SHUTDOWN;

void app_powerManager_setState(PowerManagerState state);

PowerManagerState app_powerManager_getState();
