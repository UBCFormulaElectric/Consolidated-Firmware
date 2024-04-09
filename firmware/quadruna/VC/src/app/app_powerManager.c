#include "io_efuse.h"
#include "app_powerManager.h"
#include "app_canTx.h"

static PowerStateConfig power_manager_config;

void app_powerManager_updateConfig(PowerStateConfig new_power_manager_config)
{
    bool efuses[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

const int   DEBOUNCE_TIME        = 100;
const float TIMER_ATTEMPTS_LIMIT = 100.0;

typedef struct
{
    RetryConfig retry_configs[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static RetryData retry_data[NUM_EFUSE_CHANNELS];

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = { .retry_configs = { [EFUSE_CHANNEL_SHDN]   = { .efuse_state    = true,
                                                                               .retry_protocol = RETRY_PROTOCOL_SHDN,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_LV]     = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_LV,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_PUMP]   = { .efuse_state    = true,
                                                                               .retry_protocol = RETRY_PROTOCOL_PUMP,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_AUX]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_NONE,
                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_INV_R]  = { .efuse_state    = false,
                                                                               .retry_protocol = RETRY_PROTOCOL_INV_R,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_INV_L]  = { .efuse_state    = false,
                                                                               .retry_protocol = RETRY_PROTOCOL_INV_L,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_TELEM]  = { .efuse_state    = true,
                                                                               .retry_protocol = RETRY_PROTOCOL_NONE,
                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_BUZZER] = { .efuse_state    = false,
                                                                               .retry_protocol = RETRY_PROTOCOL_NONE,
                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 } } },
    [POWER_MANAGER_DRIVE]    = { .retry_configs = { [EFUSE_CHANNEL_SHDN]   = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_SHDN,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_LV]     = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_LV,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_PUMP]   = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_PUMP,
                                                                               .retry_attempts_limit = 3,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_AUX]    = { .efuse_state          = false,
                                                                               .retry_protocol       = RETRY_PROTOCOL_NONE,
                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_INV_R]  = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_INV_R,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_INV_L]  = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_INV_L,
                                                                               .retry_attempts_limit = 1,
                                                                               .min_needed_current   = 0.5 },
                                                    [EFUSE_CHANNEL_TELEM]  = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_NONE,
                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 },
                                                    [EFUSE_CHANNEL_BUZZER] = { .efuse_state          = true,
                                                                               .retry_protocol       = RETRY_PROTOCOL_NONE,
                                                                               .retry_attempts_limit = 0,
                                                                               .min_needed_current   = 0 } } }
};

static PowerManagerState current_power_state = POWER_MANAGER_SHUTDOWN;

void app_powerManager_init()
{
    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        retry_data[efuse].protocol_state = PROTOCOL_STATE_OFF;
        retry_data[efuse].retry_attempts = 0;
        retry_data[efuse].current_timer_attempts = 0;
        retry_data[efuse].current_sum    = 0.0;
        retry_data[efuse].debounce_timer_attempts = 0;
    }
}

void app_powerManager_setState(PowerManagerState state)
{
    current_power_state = state;

    for (int efuse = 0; efuse < NUM_EFUSE_CHANNELS; efuse++)
    {
        bool not_in_retry_protocol = retry_data[efuse].protocol_state != PROTOCOL_STATE_DEPENDENCY_WAITING;
        bool is_efuse_on           = power_states_config[state].retry_configs[efuse].efuse_state;
        if (not_in_retry_protocol)
        {
            io_efuse_setChannel(efuse, is_efuse_on);
        }
        if (!is_efuse_on)
        {
            retry_data[efuse].protocol_state = PROTOCOL_STATE_OFF;
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
            // waiting for debounce time to pass, then efuse on
            case PROTOCOL_STATE_DEBOUNCE:
                if (efuse_retry_data->debounce_timer_attempts == DEBOUNCE_TIME)
                {
                    io_efuse_standbyReset(efuse);
                    io_efuse_setChannel(efuse, true);

                    efuse_retry_data->protocol_state = PROTOCOL_STATE_CALC_AVG;
                    efuse_retry_data->debounce_timer_attempts = 0;
                }
                else
                {
                    efuse_retry_data->debounce_timer_attempts++;
                }
            // This efuse is waiting for a different efuse to finish retry protocol
            case PROTOCOL_STATE_DEPENDENCY_WAITING:
                break;
            // Going through the retry protocol, getting current total
            case PROTOCOL_STATE_CALC_AVG:
            {
                efuse_retry_data->current_sum += io_efuse_getChannelCurrent(efuse);
                efuse_retry_data->current_timer_attempts++;
                // Check if timer attempts reached the limit
                if (efuse_retry_data->current_timer_attempts == TIMER_ATTEMPTS_LIMIT)
                {
                    efuse_retry_data->protocol_state = PROTOCOL_STATE_CALC_DONE;
                }
                break;
            }
            // Finished calculating current avg
            case PROTOCOL_STATE_CALC_DONE:
            {
                float avg = efuse_retry_data->current_sum / TIMER_ATTEMPTS_LIMIT;
                if (avg <= efuse_retry_config->min_needed_current)
                {
                    if (efuse_retry_data->retry_attempts == efuse_retry_config->retry_attempts_limit)
                    {
                        app_canTx_VC_Efuse_set(efuse_retry_config->efuse_state);
                        app_canTx_VC_Fault_Count_set((long unsigned int)efuse_retry_data->retry_attempts);
                    }
                    else
                    {
                        go_to_init_state =
                            app_retryHandler_start(efuse_retry_config->retry_protocol, efuse_retry_config, retry_data);
                        efuse_retry_data->retry_attempts++;
                    }
                }
                else if (efuse_retry_data->retry_attempts > 0)
                {
                    app_retryHandler_success(efuse_retry_config->retry_protocol, efuse_retry_config, retry_data);
                }
                efuse_retry_data->current_sum    = 0;
                efuse_retry_data->current_timer_attempts = 0;
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
                    efuse_retry_data->current_timer_attempts++;
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
    return power_manager_config;
}

bool app_powerManager_getEfuse(EfuseChannel channel)
{
    return power_manager_config.efuses[channel];
}