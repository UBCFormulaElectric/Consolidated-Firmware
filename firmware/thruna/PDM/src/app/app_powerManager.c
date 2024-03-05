#include "app_powerManager.h"
#include "app_canRx.h"
#include "app_retryHandler.h"
#include "app_canTx.h"
#include <stdio.h>

typedef struct
{
    RetryConfig retry_configs[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = {
        .retry_configs = {
            [EFUSE_CHANNEL_AIR] = {true, RETRY_PROTOCOL_AIR, 1},
            [EFUSE_CHANNEL_LVPWR] = {true, RETRY_PROTOCOL_LV, 1},
            [EFUSE_CHANNEL_EMETER] = {false, RETRY_PROTOCOL_EMETER, 3},
            [EFUSE_CHANNEL_AUX] = {false, RETRY_PROTOCOL_AUX, 3},
            [EFUSE_CHANNEL_DRS] = {false, RETRY_PROTOCOL_DRS, 3},
            [EFUSE_CHANNEL_FAN] = {false, RETRY_PROTOCOL_FANS, 3},
            [EFUSE_CHANNEL_DI_LHS] = {false, RETRY_PROTOCOL_INVERTERS, 1},
            [EFUSE_CHANNEL_DI_RHS] = {false, RETRY_PROTOCOL_INVERTERS, 1},
        }
    },
    [POWER_MANAGER_DRIVE] = {
        .retry_configs = {
            [EFUSE_CHANNEL_AIR] = {true, RETRY_PROTOCOL_AIR, 1},
            [EFUSE_CHANNEL_LVPWR] = {true, RETRY_PROTOCOL_LV, 1},
            [EFUSE_CHANNEL_EMETER] = {false, RETRY_PROTOCOL_EMETER, 3},
            [EFUSE_CHANNEL_AUX] = {false, RETRY_PROTOCOL_AUX, 3},
            [EFUSE_CHANNEL_DRS] = {false, RETRY_PROTOCOL_DRS, 3},
            [EFUSE_CHANNEL_FAN] = {true, RETRY_PROTOCOL_FANS, 3},
            [EFUSE_CHANNEL_DI_LHS] = {true, RETRY_PROTOCOL_INVERTERS, 1},
            [EFUSE_CHANNEL_DI_RHS] = {true, RETRY_PROTOCOL_INVERTERS, 1},
        }
    }
};

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel(efuse, power_states_config[state].retry_configs[efuse].state);
    }
}

void app_powerManager_check_efuses(PowerManagerState state) {
    TimerChannel timer;
    TimerState timer_state;
    app_timer_init(&timer, CHECK_TIME);

    bool success = false;
    int  efuse   = 0;
    int  attempt = 0;

    while (efuse < NUM_EFUSE_CHANNELS) {
        RetryConfig retry_config = power_states_config[state].retry_configs[efuse];

        while (timer_state != TIMER_STATE_EXPIRED)
        {
            timer_state = app_timer_updateAndGetState(&timer);
        }
        success = retry_config.state ? io_efuse_getChannelCurrent(efuse) > FAULT_CURRENT_THRESHOLD : true;

        if (success)
        {
            efuse++;
            attempt = 0;
        }
        else if (attempt == retry_config.limit)
        {
            app_canTx_PDM_EfuseFault_set(true);
            success = true;
            break;
        }
        else if (!success) {
            apply_retry_protocol(retry_config.protocol, power_states_config[state].retry_configs, success);
        }
        else
        {
            attempt++;
        }
    }
}

PowerManagerState app_powerManager_getState()
{
    return current_power_state;
}
