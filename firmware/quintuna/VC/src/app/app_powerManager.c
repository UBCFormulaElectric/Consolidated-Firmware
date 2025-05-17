#include "app_powerManager.h"
#include "app_timer.h"
#include "io_loadswitch.h"
#include "io_loadswitches.h"
#include <stdbool.h>
#include <stdint.h>

static PowerState    power_manager_state;
static TimerChannel *sequencing_timer;

typedef union
{
    const ST_LoadSwitch *st;
    const TI_LoadSwitch *ti;
} Loadswitch;

typedef struct
{
    Loadswitch loadswitch;
    uint8_t    retry_num;
} RetryProtocol;

static RetryProtocol efuses_retry_state[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV]   = { .loadswitch.st = &inv_rsm_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_RSM]     = { .loadswitch.st = &inv_rsm_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_BMS]     = { .loadswitch.st = &inv_bms_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_R_INV]   = { .loadswitch.st = &inv_bms_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_DAM]     = { .loadswitch.st = &front_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_FRONT]   = { .loadswitch.st = &front_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_RL_PUMP] = { .loadswitch.ti = &rl_pump_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_RR_PUMP] = { .loadswitch.ti = &rr_pump_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_F_PUMP]  = { .loadswitch.ti = &f_pump_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_L_RAD]   = { .loadswitch.st = &rad_fan_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_R_RAD]   = { .loadswitch.st = &rad_fan_loadswitch, .retry_num = 0 }
};

static bool efuse_blown_status [NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV]   = false,
    [EFUSE_CHANNEL_RSM]     = false,
    [EFUSE_CHANNEL_BMS]     = false,
    [EFUSE_CHANNEL_R_INV]   = false,
    [EFUSE_CHANNEL_DAM]     = false,
    [EFUSE_CHANNEL_FRONT]   = false,
    [EFUSE_CHANNEL_RL_PUMP] = false,
    [EFUSE_CHANNEL_RR_PUMP] = false,
    [EFUSE_CHANNEL_F_PUMP]  = false,
    [EFUSE_CHANNEL_L_RAD]   = false,
    [EFUSE_CHANNEL_R_RAD]   = false
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
        else if (efuses_retry_state[current_efuse_sequence].retry_num <= efuse_retry_limit)
        {
            // This to indicate that we want the efuse to be off when its on so just turn it off
            if (!desired_efuse_state)
            {
                io_loadswitch_setChannel(&efuse_channels[current_efuse_sequence], desired_efuse_state);
                break;
            }
            // If we dont know the if the efuse is blown check if it is however if we know its already blown dont check
            if(!efuse_blown_status[current_efuse_sequence]){
                if (current_efuse_sequence <= EFUSE_CHANNEL_F_PUMP && current_efuse_sequence >= EFUSE_CHANNEL_RL_PUMP)
                {
                    efuse_blown_status[current_efuse_sequence] = io_TILoadswitch_Status(efuses_retry_state[current_efuse_sequence].loadswitch.ti);
                }
                else{
                    efuse_blown_status[current_efuse_sequence] = io_STLoadswitch_Status(efuses_retry_state[current_efuse_sequence].loadswitch.st);
                }
            }
            // After this is we begin power sequencing logic as we want to turn on the loads
            if (sequencing_timer->state == TIMER_STATE_IDLE && efuse_retry_timeout != 0)
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

                //If we sucessfully retried and the efuse was blown increment the retry
                if (efuse_blown_status[current_efuse_sequence]){
                    efuses_retry_state[current_efuse_sequence].retry_num++;
                    efuse_blown_status[current_efuse_sequence] = false;
                }
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
