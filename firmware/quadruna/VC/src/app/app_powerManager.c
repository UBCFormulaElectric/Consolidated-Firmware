#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canTx.h"

<<<<<<< HEAD
static PowerStateConfig power_manager_config;

void app_powerManager_updateConfig(PowerStateConfig new_power_manager_config)
{
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

const int   DEBOUNCE_TIME        = 100;
const float TIMER_ATTEMPTS_LIMIT = 100.0;
=======
const int NUM_DEBOUNCE_CYCLES = 100;
>>>>>>> 5b2de999 (code review changes)

typedef struct
{
    RetryConfig retry_configs[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static RetryData retry_data[NUM_EFUSE_CHANNELS];

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = { .retry_configs = { [EFUSE_CHANNEL_SHDN]   = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_LV]     = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_PUMP]   = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_AUX]    = { .is_efuse_on = false,

                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_INV_R]  = { .is_efuse_on = false,

                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_INV_L]  = { .is_efuse_on = false,

                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_TELEM]  = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_BUZZER] = { .is_efuse_on = false,

                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 } } },
    [POWER_MANAGER_DRIVE]    = { .retry_configs = { [EFUSE_CHANNEL_SHDN]   = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_LV]     = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_PUMP]   = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_AUX]    = { .is_efuse_on = false,

                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_INV_R]  = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_INV_L]  = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_TELEM]  = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_BUZZER] = { .is_efuse_on = true,

                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 } } }
};

static PowerManagerState current_power_state = POWER_MANAGER_SHUTDOWN;

void app_powerManager_init()
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        retry_data[efuse].protocol_state = PROTOCOL_STATE_OFF;
    }
}

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        bool not_in_retry_protocol = retry_data[efuse].protocol_state != PROTOCOL_STATE_DEPENDENCY_WAITING ||
                                     retry_data[efuse].protocol_state == PROTOCOL_STATE_DEBOUNCE ||
                                     retry_data[efuse].protocol_state == PROTOCOL_STATE_FAULTED;
        if (not_in_retry_protocol)
        {
            io_efuse_setChannel(efuse, power_states_config[state].retry_configs[efuse].is_efuse_on);
        }
    }
}

bool app_powerManager_checkEfuses(PowerManagerState state)
{
    bool go_to_init_state = false;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        RetryData         *efuse_retry_data   = &retry_data[efuse];
        const RetryConfig *efuse_retry_config = &power_states_config[state].retry_configs[efuse];

        switch (efuse_retry_data->protocol_state)
        {
            // Handle waiting for debounce time to pass and then turn on the efuse
            case PROTOCOL_STATE_DEBOUNCE:
                if (efuse_retry_data->timer_attempts == NUM_DEBOUNCE_CYCLES)
                {
                    io_efuse_standbyReset(efuse);
                    io_efuse_setChannel(efuse, true);

                    efuse_retry_data->protocol_state = PROTOCOL_STATE_CALC_AVG;
                    efuse_retry_data->timer_attempts = 0;
                }
                else
                {
                    efuse_retry_data->timer_attempts++;
                }
            // Handle efuse dependant that is waiting for an efuse in protocol
            case PROTOCOL_STATE_FAULTED:
            case PROTOCOL_STATE_DEPENDENCY_WAITING:
                break;
            // Handle going through the retry protocol and calculating current total
            case PROTOCOL_STATE_CALC_AVG:
            {
                efuse_retry_data->current_sum += io_efuse_getChannelCurrent(efuse);
                efuse_retry_data->timer_attempts++;
                // Check if timer attempts reached the limit
                if (efuse_retry_data->timer_attempts == NUM_DEBOUNCE_CYCLES)
                {
                    efuse_retry_data->protocol_state = PROTOCOL_STATE_CALC_DONE;
                }
                break;
            }
            // Handle finishing calculating current average
            case PROTOCOL_STATE_CALC_DONE:
            {
                float avg = efuse_retry_data->current_sum / (float)NUM_DEBOUNCE_CYCLES;

                // Calculate average and check if it's less than the minimum needed current to go into protocol
                if (avg <= efuse_retry_config->min_needed_current)
                {
                    // Reached retry attempts limit or start retry protocol
                    if (efuse_retry_data->retry_attempts == efuse_retry_config->retry_attempts_limit)
                    {
                        app_canTx_VC_FaultedEfuse_set(efuse_retry_config->is_efuse_on);
                        app_canTx_VC_EFuseFaultCount_set((long unsigned int)efuse_retry_data->retry_attempts);
                        efuse_retry_data->protocol_state = PROTOCOL_STATE_FAULTED;
                    }
                    else
                    {
                        go_to_init_state = app_retryHandler_start(efuse, efuse_retry_config, retry_data);
                        efuse_retry_data->retry_attempts++;
                    }
                }
                // Successful retry protocol after previous retry attempts
                else if (efuse_retry_data->retry_attempts > 0)
                {
                    app_retryHandler_success(efuse, efuse_retry_config, retry_data);
                    efuse_retry_data->retry_attempts = 0;
                }
                // Efuse has good current after first current count
                else
                {
                    efuse_retry_data->protocol_state = PROTOCOL_STATE_OFF;
                }

                efuse_retry_data->current_sum    = 0;
                efuse_retry_data->timer_attempts = 0;
                break;
            }
            // Handle efuse not in a retry protocol, check if it should be
            case PROTOCOL_STATE_OFF:
            {
                bool is_efuse_faulting = efuse_retry_config->is_efuse_on &&
                                         io_efuse_getChannelCurrent(efuse) < efuse_retry_config->min_needed_current;
                if (is_efuse_faulting)
                {
                    efuse_retry_data->protocol_state = PROTOCOL_STATE_CALC_AVG;
                    efuse_retry_data->current_sum    = 0;
                    efuse_retry_data->timer_attempts = 0;
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
<<<<<<< HEAD
    return power_manager_config;
}

bool app_powerManager_getEfuse(EfuseChannel channel)
{
    return power_manager_config.efuses[channel];
}
=======
    return current_power_state;
}
>>>>>>> e93d5d52 (added comments)
