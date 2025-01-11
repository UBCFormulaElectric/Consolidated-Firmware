#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canTx.h"

static PowerStateConfig power_manager_config;

void app_powerManager_updateConfig(PowerStateConfig new_power_manager_config)
{
    power_manager_config = new_power_manager_config;
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel((EfuseChannel)efuse, power_manager_config.efuses[efuse]);
    }
}

PowerStateConfig app_powerManager_getConfig(void)
{
    return power_manager_config;
}

bool app_powerManager_getEfuse(EfuseChannel channel)
{
    return power_manager_config.efuses[channel];
}