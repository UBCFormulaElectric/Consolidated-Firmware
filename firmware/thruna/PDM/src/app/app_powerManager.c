#include "app_powerManager.h"
#include "app_canRx.h"
#include "app_retryHandler.h"
#include "app_canTx.h"
#include <stdio.h>

typedef struct
{
    RetryConfig retry_configs[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static RetryData retry_data[NUM_EFUSE_CHANNELS];

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = {
        .retry_configs = {
            [EFUSE_CHANNEL_AIR] = {true, RETRY_PROTOCOL_AIR, 1},
            [EFUSE_CHANNEL_LVPWR] = {true, RETRY_PROTOCOL_LV, 1},
            [EFUSE_CHANNEL_EMETER] = {false, RETRY_PROTOCOL_EMETER, 3},
            [EFUSE_CHANNEL_AUX] = {false, RETRY_PROTOCOL_AUX, 3},
            [EFUSE_CHANNEL_DRS] = {false, RETRY_PROTOCOL_DRS, 3},
            [EFUSE_CHANNEL_FAN] = {false, RETRY_PROTOCOL_FANS, 3},
            [EFUSE_CHANNEL_DI_LHS] = {false, RETRY_PROTOCOL_DI_LHS, 1},
            [EFUSE_CHANNEL_DI_RHS] = {false, RETRY_PROTOCOL_DI_RHS, 1},
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
            [EFUSE_CHANNEL_DI_LHS] = {true, RETRY_PROTOCOL_DI_LHS, 1},
            [EFUSE_CHANNEL_DI_RHS] = {true, RETRY_PROTOCOL_DI_RHS, 1},
        }
    }
};

void app_powerManager_init()
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        retry_data[efuse].retry_state          = RETRY_STATE_OFF;
        retry_data[efuse].retry_attempts       = 0;
        retry_data[efuse].timer_attempts       = 0;
        retry_data[efuse].timer_attempts_limit = 3;
        retry_data[efuse].current_sum          = 0.0;
    }
}

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        io_efuse_setChannel(efuse, power_states_config[state].retry_configs[efuse].efuse_state);
    }
}

void app_powerManager_check_efuses(PowerManagerState state)
{
    int efuse = 0;

    while (efuse < NUM_EFUSE_CHANNELS)
    {
        RetryData         *efuse_retry_data   = &retry_data[efuse];
        const RetryConfig *efuse_retry_config = &power_states_config[state].retry_configs[efuse];

        // Check if timer attempts reached the limit
        if (efuse_retry_data->timer_attempts == efuse_retry_data->timer_attempts_limit)
        {
            efuse_retry_data->retry_state = RETRY_STATE_EXPIRED;
        }

        switch (efuse_retry_data->retry_state)
        {
            case RETRY_STATE_WAITING:
                break;
            case RETRY_STATE_RUNNING:
            {
                efuse_retry_data->current_sum += io_efuse_getChannelCurrent(efuse);
                efuse_retry_data->timer_attempts++;
                break;
            }
            case RETRY_STATE_EXPIRED:
            {
                float avg = efuse_retry_data->current_sum / (float)efuse_retry_data->timer_attempts_limit;
                if (avg <= FAULT_CURRENT_THRESHOLD)
                {
                    if (efuse_retry_data->retry_attempts == efuse_retry_config->retry_attempts_limit)
                    {
                        app_canTx_PDM_EfuseFault_set(true);
                        return;
                    }
                    retry_handler_start(efuse_retry_config->retry_protocol, efuse_retry_config, retry_data);
                    efuse_retry_data->retry_attempts++;
                }
                else
                {
                    retry_handler_recover(efuse_retry_config->retry_protocol, efuse_retry_config, retry_data);
                }
                efuse_retry_data->current_sum    = 0;
                efuse_retry_data->timer_attempts = 0;
                break;
            }
            case RETRY_STATE_OFF:
            {
                bool is_efuse_faulting =
                    efuse_retry_config->efuse_state && io_efuse_getChannelCurrent(efuse) < FAULT_CURRENT_THRESHOLD;
                if (is_efuse_faulting)
                {
                    efuse_retry_data->retry_state = RETRY_STATE_RUNNING;
                    efuse_retry_data->current_sum += io_efuse_getChannelCurrent(efuse);
                    efuse_retry_data->timer_attempts++;
                }
                break;
            }
            default:
                break;
        }
        efuse++;
    }
}

PowerManagerState app_powerManager_getState()
{
    return current_power_state;
}
