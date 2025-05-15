#include "app_powerManager.h"
#include "app_timer.h"
#include "io_loadswitch.h"
#include "io_loadswitches.h"
#include <stdint.h>

static PowerState    power_manager_state;
static TimerChannel *sequencing_timer;

static uint8_t efuses_retry_state[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV] = 0,  [EFUSE_CHANNEL_RSM] = 0,   [EFUSE_CHANNEL_BMS] = 0,     [EFUSE_CHANNEL_R_INV] = 0,
    [EFUSE_CHANNEL_DAM] = 0,    [EFUSE_CHANNEL_FRONT] = 0, [EFUSE_CHANNEL_RL_PUMP] = 0, [EFUSE_CHANNEL_RR_PUMP] = 0,
    [EFUSE_CHANNEL_F_PUMP] = 0, [EFUSE_CHANNEL_L_RAD] = 0, [EFUSE_CHANNEL_R_RAD] = 0
};

void app_powerManager_updateConfig(PowerState new_power_manager_config)
{
    power_manager_state = new_power_manager_config;
}

void app_powerManager_EfuseProtocolTick_100Hz(void)
{
    for (LoadswitchChannel current_efuse_sequence = 0; current_efuse_sequence < NUM_EFUSE_CHANNELS;
         current_efuse_sequence++)
    {
        // check if the efuse is supposed to be on or off
        bool desired_efuse_state = power_manager_state.efuses[current_efuse_sequence].efuse_enable;
        bool efuse_valid_state =
            io_loadswitch_isChannelEnabled(&efuse_channels[current_efuse_sequence]) == desired_efuse_state;

        uint8_t efuse_retry_timeout = power_manager_state.efuses[current_efuse_sequence].timeout;
        uint8_t efuse_retry_limit   = power_manager_state.efuses[current_efuse_sequence].max_retry;

        if (efuse_valid_state)
        {
            continue;
        }
        else if (efuses_retry_state[current_efuse_sequence] <= efuse_retry_limit)
        {
            // This to indicate that we want the efuse to be off when its on so just turn it off
            if (!desired_efuse_state)
            {
                io_loadswitch_setChannel(&efuse_channels[current_efuse_sequence], desired_efuse_state);
            }
            // After this is we begin power sequencing logic as we want to turn on the loads
            else if (sequencing_timer->state == TIMER_STATE_IDLE && efuse_retry_timeout != 0)
            {
                app_timer_init(sequencing_timer, power_manager_state.efuses[current_efuse_sequence].timeout);
                app_timer_restart(sequencing_timer);
            }
            else if (sequencing_timer->state == TIMER_STATE_EXPIRED || efuse_retry_timeout == 0)
            {
                io_loadswitch_setChannel(
                    &efuse_channels[current_efuse_sequence],
                    power_manager_state.efuses[current_efuse_sequence].efuse_enable);
                app_timer_stop(sequencing_timer);
                efuses_retry_state[current_efuse_sequence]++;
            }
        }
        break;
    }
}

PowerState app_powerManager_getConfig(void)
{
    return power_manager_state;
}

bool app_powerManager_getEfuse(LoadswitchChannel channel)
{
    return power_manager_state.efuses[channel].efuse_enable;
}
