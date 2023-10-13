#include "App_PowerManager.h"

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_CONTRACTOR_OPEN] = {
        .efuses = {
            [EFUSE_CHANNEL_AIR] = false,
            [EFUSE_CHANNEL_LVPWR] = false,
            [EFUSE_CHANNEL_EMETER] = false,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_DRS] = false,
            [EFUSE_CHANNEL_FAN] = false,
            [EFUSE_CHANNEL_DI_LHS] = false,
            [EFUSE_CHANNEL_DI_RHS] = false,
        }
    },
    [POWER_MANAGER_CONTRACTOR_CLOSED] = {
        .efuses = {
            [EFUSE_CHANNEL_AIR] = true,
            [EFUSE_CHANNEL_LVPWR] = true,
            [EFUSE_CHANNEL_EMETER] = true,
            [EFUSE_CHANNEL_AUX] = true,
            [EFUSE_CHANNEL_DRS] = true,
            [EFUSE_CHANNEL_FAN] = true,
            [EFUSE_CHANNEL_DI_LHS] = true,
            [EFUSE_CHANNEL_DI_RHS] = true,
        }
    }
};

void PowerManager_Init()
{
    PowerManager_Init(POWER_MANAGER_CONTRACTOR_OPEN);

    Io_Efuse_SetChannel(EFUSE_CHANNEL_LVPWR, POWER_MANAGER_CONTRACTOR_CLOSED);
    Io_Efuse_SetChannel(EFUSE_CHANNEL_DI_LHS, POWER_MANAGER_CONTRACTOR_CLOSED);
    Io_Efuse_SetChannel(EFUSE_CHANNEL_DI_RHS, POWER_MANAGER_CONTRACTOR_CLOSED);
    Io_Efuse_SetChannel(EFUSE_CHANNEL_AIR, POWER_MANAGER_CONTRACTOR_CLOSED);
}

void PowerManager_SetState(PowerManagerState state)
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        Io_Efuse_SetChannel(efuse, power_states_config[state].efuses[efuse]);
    }
}
