#include "App_PowerManager.h"

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_CONTRACTOR_SHUTDOWN] = {
        .efuses = {
            [EFUSE_CHANNEL_AIR] = true,
            [EFUSE_CHANNEL_LVPWR] = true,
            [EFUSE_CHANNEL_EMETER] = false,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_DRS] = false,
            [EFUSE_CHANNEL_FAN] = false,
            [EFUSE_CHANNEL_DI_LHS] = false,
            [EFUSE_CHANNEL_DI_RHS] = false,
        }
    },
    [POWER_MANAGER_CONTRACTOR_DRIVE] = {
        .efuses = {
            [EFUSE_CHANNEL_AIR] = true,
            [EFUSE_CHANNEL_LVPWR] = true,
            [EFUSE_CHANNEL_EMETER] = false,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_DRS] = false,
            [EFUSE_CHANNEL_FAN] = true,
            [EFUSE_CHANNEL_DI_LHS] = true,
            [EFUSE_CHANNEL_DI_RHS] = true,
        }
    }
};

void app_powerManager_init()
{
    app_powerManager_setState(POWER_MANAGER_CONTRACTOR_DRIVE);
}

void app_powerManager_setState(PowerManagerState state)
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel(efuse, power_states_config[state].efuses[efuse]);
    }
}
