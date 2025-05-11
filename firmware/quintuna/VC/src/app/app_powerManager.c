#include "app_powerManager.h"
#include "io_loadswitch.h"
#include "io_loadswitches.h"

static PowerState power_manager_state;

void app_powerManager_updateConfig(PowerState new_power_manager_config)
{
    power_manager_state = new_power_manager_config;
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_loadswitch_setChannel(&efuse_channels[efuse], power_manager_state.efuses[efuse]);
    }
}

PowerState app_powerManager_getConfig(void)
{
    return power_manager_state;
}

bool app_powerManager_getEfuse(LoadswitchChannel channel)
{
    return power_manager_state.efuses[channel];
}

void app_powerManager_setEfuse(Efuse channel, bool enable)
{
    io_loadswitch_setChannel(&channel, enable);
}