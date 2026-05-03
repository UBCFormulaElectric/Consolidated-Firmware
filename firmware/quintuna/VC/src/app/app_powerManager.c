#include "app_powerManager.h"

#include "app_timer.h"
#include "io_efuses.h"
#include "app_canAlerts.h"

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

static PowerManagerConfig power_manager_state;
static TimerChannel       sequencing_timer;

static uint8_t efuses_retry_num[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV] = 0, [EFUSE_CHANNEL_RSM] = 0,   [EFUSE_CHANNEL_BMS] = 0,     [EFUSE_CHANNEL_R_INV] = 0,
    [EFUSE_CHANNEL_DAM] = 0,   [EFUSE_CHANNEL_FRONT] = 0, [EFUSE_CHANNEL_RL_PUMP] = 0, [EFUSE_CHANNEL_R_RAD] = 0
};

void app_powerManager_updateConfig(const PowerManagerConfig new_power_manager_config)
{
    power_manager_state = new_power_manager_config;

    power_manager_state.efuse_configs[EFUSE_CHANNEL_RL_PUMP].efuse_enable =
        !app_canAlerts_VC_Info_PcmUnderVoltage_get();
    power_manager_state.efuse_configs[EFUSE_CHANNEL_R_RAD].efuse_enable = !app_canAlerts_VC_Info_PcmUnderVoltage_get();
}

void app_powerManager_EfuseProtocolTick_100Hz(void)
{
    switch (app_timer_updateAndGetState(&sequencing_timer))
    {
        case TIMER_STATE_RUNNING:
        default:
            break;
        case TIMER_STATE_EXPIRED:
            // timeout expired or no timeout
            app_timer_stop(&sequencing_timer);
            // ReSharper disable once CppRedundantEmptyStatement
            // TODO: what the fuck
            __attribute__((fallthrough));
        case TIMER_STATE_IDLE:
            for (EfuseChannel current_efuse_sequence = 0;
                 current_efuse_sequence < NUM_EFUSE_CHANNELS &&
                 app_timer_updateAndGetState(&sequencing_timer) == TIMER_STATE_IDLE;
                 current_efuse_sequence++)
            {
                const Efuse *channel = efuse_channels[current_efuse_sequence];

                // check if the efuse is supposed to be on or off
                const bool desired_efuse_state = power_manager_state.efuse_configs[current_efuse_sequence].efuse_enable;
                if (io_efuse_isChannelEnabled(channel) == desired_efuse_state)
                {
                    // efuse is fine
                    continue;
                }

                // todo check this when incrementing the efuse failure
                if (efuses_retry_num[current_efuse_sequence] >
                    power_manager_state.efuse_configs[current_efuse_sequence].max_retry)
                {
                    // todo over the retry limit activities
                    // there is something with the system so put car into fault state
                    break;
                }

                if (desired_efuse_state == false)
                {
                    // case 1: on and trying to turn off
                    io_efuse_setChannel(channel, desired_efuse_state);
                    continue;
                }
                // case 2: off and trying to turn on
                // we update the efuse blown status here because this only shows up when we want it on
                if (!io_efuse_ok(channel)) // todo remove this state?
                {
                    efuses_retry_num[current_efuse_sequence]++;
                }

                // If we dont know the if the efuse is blown check if it is however if we know its already blown
                // dont check After this is we begin power sequencing logic as we want to turn on the loads
                const uint8_t efuse_retry_timeout = power_manager_state.efuse_configs[current_efuse_sequence].timeout;
                if (efuse_retry_timeout != 0)
                {
                    // start the timeout
                    app_timer_init(&sequencing_timer, efuse_retry_timeout);
                    app_timer_restart(&sequencing_timer);
                }
                io_efuse_setChannel(channel, power_manager_state.efuse_configs[current_efuse_sequence].efuse_enable);
            }
            break;
    }
}

#ifdef TARGET_TEST
PowerManagerConfig app_powerManager_getConfig(void)
{
    return power_manager_state;
}

bool app_powerManager_getEfuse(const LoadswitchChannel channel)
{
    return power_manager_state.efuse_configs[channel].efuse_enable;
}
#endif
