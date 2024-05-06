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

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
    bool pcm;
} PowerStateConfig;

void app_powerManager_updateState(PowerStateConfig state);

PowerManagerState app_powerManager_getState(void);

void app_powerManager_updateEfuse(EfuseChannel channel, bool val);

void app_powerManager_updatePcm(bool val);
