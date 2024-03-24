#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canRx.h"

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = {
        .efuses = {
            [EFUSE_CHANNEL_SHDN] = true,
            [EFUSE_CHANNEL_LV] = true,
            [EFUSE_CHANNEL_PUMP] = false,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_INV_R] = false,
            [EFUSE_CHANNEL_INV_L] = false,
            [EFUSE_CHANNEL_TELEM] = true,
            [EFUSE_CHANNEL_BUZZER] = false,
        }
    },
    [POWER_MANAGER_DRIVE] = {
        .efuses = {
            [EFUSE_CHANNEL_SHDN] = false,
            [EFUSE_CHANNEL_LV] = true,
            [EFUSE_CHANNEL_PUMP] = true,
            [EFUSE_CHANNEL_AUX] = true,
            [EFUSE_CHANNEL_INV_R] = true,
            [EFUSE_CHANNEL_INV_L] = true,
            [EFUSE_CHANNEL_TELEM] = true,
            [EFUSE_CHANNEL_BUZZER] = true,
        }
    }
};

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel(efuse, power_states_config[state].efuses[efuse]);
    }
}

PowerManagerState app_powerManager_getState()
{
    return current_power_state;
}