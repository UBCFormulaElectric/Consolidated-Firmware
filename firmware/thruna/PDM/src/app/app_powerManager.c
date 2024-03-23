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
    [POWER_MANAGER_SHUTDOWN] = { .retry_configs = { [EFUSE_CHANNEL_AIR]    = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_AIR,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_LVPWR]  = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_LV,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_EMETER] = { .efuse_state    = false,
                                                                               .retry_protocol = RETRY_PROTOCOL_EMETER,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_AUX]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_AUX,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_DRS]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_DRS,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_FAN]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_FANS,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_DI_LHS] = { .efuse_state    = false,
                                                                               .retry_protocol = RETRY_PROTOCOL_DI_LHS,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_DI_RHS] = { .efuse_state    = false,
                                                                               .retry_protocol = RETRY_PROTOCOL_DI_RHS,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 } } },
    [POWER_MANAGER_DRIVE]    = { .retry_configs = { [EFUSE_CHANNEL_AIR]    = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_AIR,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_LVPWR]  = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_LV,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_EMETER] = { .efuse_state    = true,
                                                                               .retry_protocol = RETRY_PROTOCOL_EMETER,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_AUX]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_AUX,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_DRS]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_DRS,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_FAN]    = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_FANS,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_DI_LHS] = { .efuse_state    = true,
                                                                               .retry_protocol = RETRY_PROTOCOL_DI_LHS,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 },
                                                    [EFUSE_CHANNEL_DI_RHS] = { .efuse_state    = true,
                                                                               .retry_protocol = RETRY_PROTOCOL_DI_RHS,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5,
                                                                               .timer_attempts_limit = 200 } } }
};

void app_powerManager_init()
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        retry_data[efuse].protocol_state = PROTOCOL_STATE_OFF;
        retry_data[efuse].retry_attempts = 0;
        retry_data[efuse].timer_attempts = 0;
        retry_data[efuse].current_sum    = 0.0;
    }
}

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        bool not_in_retry_protocol = retry_data[efuse].protocol_state == PROTOCOL_STATE_OFF;
        if (not_in_retry_protocol)
        {
            io_efuse_setChannel(efuse, power_states_config[state].retry_configs[efuse].efuse_state);
        }
    }
}

bool app_powerManager_check_efuses(PowerManagerState state)
{
    bool go_to_init_state = false;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        RetryData         *efuse_retry_data   = &retry_data[efuse];
        const RetryConfig *efuse_retry_config = &power_states_config[state].retry_configs[efuse];

        // Check if timer attempts reached the limit
        if (efuse_retry_data->timer_attempts == efuse_retry_config->timer_attempts_limit)
        {
            efuse_retry_data->protocol_state = PROTOCOL_STATE_DONE_CALC;
        }

        switch (efuse_retry_data->protocol_state)
        {
            // This efuse is waiting for a different efuse to finish retry protocol
            case PROTOCOL_STATE_WAITING:
                break;
            // Going through the retry protocol, getting current total
            case PROTOCOL_STATE_CALC_AVG:
            {
                efuse_retry_data->current_sum += io_efuse_getChannelCurrent(efuse);
                efuse_retry_data->timer_attempts++;
                break;
            }
            // Finished calculating current avg
            case PROTOCOL_STATE_DONE_CALC:
            {
                float avg = efuse_retry_data->current_sum / (float)efuse_retry_config->timer_attempts_limit;
                if (avg <= efuse_retry_config->min_needed_current)
                {
                    if (efuse_retry_data->retry_attempts == efuse_retry_config->retry_attempts_limit)
                    {
                        app_canTx_PDM_Efuse_set(efuse_retry_config->efuse_state);
                        app_canTx_PDM_Amount_set((long unsigned int)efuse_retry_data->retry_attempts);
                        return go_to_init_state;
                    }
                    go_to_init_state =
                        app_retry_handler_start(efuse_retry_config->retry_protocol, efuse_retry_config, retry_data);
                    efuse_retry_data->retry_attempts++;
                }
                else if (efuse_retry_data->retry_attempts > 0)
                {
                    app_retry_handler_success(efuse_retry_config->retry_protocol, efuse_retry_config, retry_data);
                }
                efuse_retry_data->current_sum    = 0;
                efuse_retry_data->timer_attempts = 0;
                break;
            }
            // Not in a retry protocol, checking if it should be
            case PROTOCOL_STATE_OFF:
            {
                bool is_efuse_faulting = efuse_retry_config->efuse_state &&
                                         io_efuse_getChannelCurrent(efuse) < efuse_retry_config->min_needed_current;
                if (is_efuse_faulting)
                {
                    efuse_retry_data->protocol_state = PROTOCOL_STATE_CALC_AVG;
                    efuse_retry_data->current_sum += io_efuse_getChannelCurrent(efuse);
                    efuse_retry_data->timer_attempts++;
                }
                break;
            }
            default:
                break;
        }
    }

    return go_to_init_state;
}

PowerManagerState app_powerManager_getState()
{
    return current_power_state;
}
