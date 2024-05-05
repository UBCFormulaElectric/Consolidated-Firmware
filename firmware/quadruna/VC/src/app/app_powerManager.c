#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canRx.h"
#include "io_pcm.h"

typedef struct
{
    bool efuses[NUM_EFUSE_CHANNELS];
    bool pcm;
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
        },
        .pcm = false,
    },
    [POWER_MANAGER_INVERTER_ON_PRE_AIR_PLUS] = {
        .efuses = {
            [EFUSE_CHANNEL_SHDN] = true,
            [EFUSE_CHANNEL_LV] = true,
            [EFUSE_CHANNEL_PUMP] = false,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_INV_R] = true,
            [EFUSE_CHANNEL_INV_L] = true,
            [EFUSE_CHANNEL_TELEM] = true,
            [EFUSE_CHANNEL_BUZZER] = false,
        },
        .pcm = false,
    },
    [POWER_MANAGER_INVERTER_ON_POST_AIR_PLUS] = {
        .efuses = {
            [EFUSE_CHANNEL_SHDN] = true,
            [EFUSE_CHANNEL_LV] = true,
            [EFUSE_CHANNEL_PUMP] = true,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_INV_R] = true,
            [EFUSE_CHANNEL_INV_L] = true,
            [EFUSE_CHANNEL_TELEM] = true,
            [EFUSE_CHANNEL_BUZZER] = false,
        },
        .pcm = true,
    },
    [POWER_MANAGER_DRIVE] = {
        .efuses = {
            [EFUSE_CHANNEL_SHDN] = false,
            [EFUSE_CHANNEL_LV] = true,
            [EFUSE_CHANNEL_PUMP] = true,
            [EFUSE_CHANNEL_AUX] = false,
            [EFUSE_CHANNEL_INV_R] = true,
            [EFUSE_CHANNEL_INV_L] = true,
            [EFUSE_CHANNEL_TELEM] = true,
            [EFUSE_CHANNEL_BUZZER] = true,
        },
        .pcm = true,
    }
};

static PowerManagerState current_power_state = POWER_MANAGER_SHUTDOWN;

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel((EfuseChannel)efuse, power_states_config[state].efuses[efuse]);
    }
    io_pcm_set(power_states_config[state].pcm);
}

PowerManagerState app_powerManager_getState(void)
{
    return current_power_state;
}
