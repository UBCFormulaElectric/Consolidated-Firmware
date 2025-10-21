#pragma once

#include <stdint.h>
#include "io/loadswitches.h"

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

void app_powerManager_init();
void app_powerManager_updateConfig(PowerManagerConfig new_power_manager_config);
void app_powerManager_EfuseProtocolTick_100Hz(void);

#ifdef TARGET_TEST
PowerManagerConfig app_powerManager_getConfig(void);
bool               app_powerManager_getEfuse(LoadswitchChannel channel);
#endif