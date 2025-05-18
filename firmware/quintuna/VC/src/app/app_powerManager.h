#pragma once

#include <stdbool.h>
#include "app_utils.h"
#include "io_loadswitch.h"
#include "io_loadswitches.h"

typedef struct
{
    bool    efuse_enable;
    uint8_t timeout;
    uint8_t max_retry;
} EfuseConfig;

typedef struct
{
    EfuseConfig efuse_configs[NUM_EFUSE_CHANNELS];
} PowerManagerConfig;

void app_powerManager_updateConfig(PowerManagerConfig new_power_manager_config);
void app_powerManager_EfuseProtocolTick_100Hz(void);

#ifdef TARGET_TEST
PowerStateConfig app_powerManager_getConfig(void);
bool             app_powerManager_getEfuse(EfuseChannel channel);
#endif