#include <stdbool.h>
#include "io_efuse.h"

typedef enum
{
    POWER_MANAGER_SHUTDOWN,
    POWER_MANAGER_INVERTER_ON_PRE_AIR_PLUS,
    POWER_MANAGER_INVERTER_ON_POST_AIR_PLUS,
    POWER_MANAGER_DRIVE,
    NUM_POWER_STATES
} PowerManagerState;

void app_powerManager_setState(PowerManagerState state);

PowerManagerState app_powerManager_getState(void);
