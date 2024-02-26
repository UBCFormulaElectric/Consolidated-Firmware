#include "app_powerManager.h"
#include "app_canRx.h"
#include "app_retryHandler.h"
#include "app_canTx.h"

typedef struct
{
    bool          efuse_state;
    RetryProtocol retry_protocol;
    int           retry_limit;
} EFuseConfig;

typedef struct
{
    EFuseConfig efuse_configs[NUM_EFUSE_CHANNELS];
} PowerStateConfig;

static const PowerStateConfig power_states_config[NUM_POWER_STATES] = {
    [POWER_MANAGER_SHUTDOWN] = {
        .efuse_configs = {
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
        .efuse_configs = {
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
    TimerChannel timer;
    app_timer_init(&timer, CHECK_TIME);
    current_power_state = state;
    bool success        = false;

    static int efuse   = 0;
    static int attempt = 0;

    while (efuse < NUM_EFUSE_CHANNELS)
    {
        EFuseConfig efuse_config = power_states_config[state].efuse_configs[efuse];
        io_efuse_setChannel(efuse, efuse_config.efuse_state);
        success = io_efuse_getChannelCurrent(efuse) > FAULT_CURRENT_THRESHOLD;
        app_timer_restart(&timer);

        if (app_timer_updateAndGetState(&timer) == TIMER_STATE_EXPIRED)
        {
            apply_retry_protocol(efuse_config.retry_protocol, success);
            if (success)
            {
                efuse++;
                attempt = 0;
            }
            else if (attempt == efuse_config.retry_limit)
            {
                app_canTx_PDM_EfuseFault_set(true);
            } else 
            {
                attempt++;
            }
        }
    }
}

PowerManagerState app_powerManager_getState()
{
    return current_power_state;
}
