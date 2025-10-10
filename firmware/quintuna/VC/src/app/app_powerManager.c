#include "app_powerManager.h"

#include "app_timer.h"
#include "io_loadswitches.h"
#include "app_canAlerts.h"

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

static PowerManagerConfig power_manager_state;
static TimerChannel       sequencing_timer;

typedef union
{
    const ST_LoadSwitch *st;
    const TI_LoadSwitch *ti;
} LoadSwitch;

typedef struct
{
    LoadSwitch loadswitch;
    uint8_t    retry_num;
} RetryProtocol;

static RetryProtocol efuses_retry_state[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_F_INV]   = { .loadswitch.st = &inv_rsm_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_RSM]     = { .loadswitch.st = &inv_rsm_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_BMS]     = { .loadswitch.st = &inv_bms_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_R_INV]   = { .loadswitch.st = &inv_bms_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_DAM]     = { .loadswitch.st = &front_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_FRONT]   = { .loadswitch.st = &front_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_RR_PUMP] = { .loadswitch.ti = &rr_pump_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_F_PUMP]  = { .loadswitch.ti = &f_pump_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_RL_PUMP] = { .loadswitch.ti = &rl_pump_loadswitch, .retry_num = 0 },
    [EFUSE_CHANNEL_R_RAD]   = { .loadswitch.st = &rad_fan_loadswitch, .retry_num = 0 }
};

void app_powerManager_updateConfig(const PowerManagerConfig new_power_manager_config)
{
    power_manager_state = new_power_manager_config;

    power_manager_state.efuse_configs[EFUSE_CHANNEL_RL_PUMP].efuse_enable =
        !app_canAlerts_VC_Info_PcmUnderVoltage_get();
    power_manager_state.efuse_configs[EFUSE_CHANNEL_R_RAD].efuse_enable = !app_canAlerts_VC_Info_PcmUnderVoltage_get();
}

// static bool STLoadswitch_Status(const ST_LoadSwitch *loadswitch)
// {
//     assert(loadswitch->efuse1 != NULL && loadswitch->efuse2 != NULL);

//     // Checking if ther is an overtemperature/short to ground condition
//     float vsenseh_efuse1 = io_loadswitch_getChannelCurrent(loadswitch->efuse1) / ADC_VOLTAGE_TO_CURRENT_A;
//     float vsenseh_efuse2 = io_loadswitch_getChannelCurrent(loadswitch->efuse2) / ADC_VOLTAGE_TO_CURRENT_A;

//     if (io_loadswitch_isChannelEnabled(loadswitch->efuse1) && vsenseh_efuse1 >= 3.0f)
//     {
//         return false;
//     }

//     if (io_loadswitch_isChannelEnabled(loadswitch->efuse2) && vsenseh_efuse2 >= 3.0f)
//     {
//         return false;
//     }

//     // Checking if there is a short to VBAT condition
//     io_STloadswitch_reset_set(loadswitch, true);

//     if (!io_loadswitch_isChannelEnabled(loadswitch->efuse1) && vsenseh_efuse1 > 3.0f)
//     {
//         return false;
//     }

//     if (!io_loadswitch_isChannelEnabled(loadswitch->efuse2) && vsenseh_efuse2 > 3.0f)
//     {
//         return false;
//     }

//     // reset the stby reset gpio to low
//     io_STloadswitch_reset_set(loadswitch, false);
//     return true;
// }

// bool is_efuse_ok(const uint8_t current_efuse_sequence)
// {
//     if (EFUSE_CHANNEL_RL_PUMP <= current_efuse_sequence && current_efuse_sequence <= EFUSE_CHANNEL_F_PUMP)
//     {
//         return io_TILoadswitch_pgood(efuses_retry_state[current_efuse_sequence].loadswitch.ti);
//     }
//     return STLoadswitch_Status(efuses_retry_state[current_efuse_sequence].loadswitch.st);
// }

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
            for (LoadswitchChannel current_efuse_sequence = 0;
                 current_efuse_sequence < NUM_EFUSE_CHANNELS &&
                 app_timer_updateAndGetState(&sequencing_timer) == TIMER_STATE_IDLE;
                 current_efuse_sequence++)
            {
                // check if the efuse is supposed to be on or off
                const bool desired_efuse_state = power_manager_state.efuse_configs[current_efuse_sequence].efuse_enable;
                if (io_loadswitch_isChannelEnabled(efuse_channels[current_efuse_sequence]) == desired_efuse_state)
                {
                    // efuse is fine
                    continue;
                }

                // todo check this when incrementing the efuse failure
                if (efuses_retry_state[current_efuse_sequence].retry_num >
                    power_manager_state.efuse_configs[current_efuse_sequence].max_retry)
                {
                    // todo over the retry limit activities
                    // there is something with the system so put car into fault state
                    break;
                }

                if (desired_efuse_state == false)
                {
                    // case 1: on and trying to turn off
                    io_loadswitch_setChannel(efuse_channels[current_efuse_sequence], desired_efuse_state);
                    continue;
                }
                // case 2: off and trying to turn on
                // we update the efuse blown status here because this only shows up when we want it on
                // if (!is_efuse_ok(current_efuse_sequence)) // todo remove this state?
                // {
                //     efuses_retry_state[current_efuse_sequence].retry_num++;
                // }

                // If we dont know the if the efuse is blown check if it is however if we know its already blown
                // dont check After this is we begin power sequencing logic as we want to turn on the loads
                const uint8_t efuse_retry_timeout = power_manager_state.efuse_configs[current_efuse_sequence].timeout;
                if (efuse_retry_timeout != 0)
                {
                    // start the timeout
                    app_timer_init(&sequencing_timer, efuse_retry_timeout);
                    app_timer_restart(&sequencing_timer);
                }
                io_loadswitch_setChannel(
                    efuse_channels[current_efuse_sequence],
                    power_manager_state.efuse_configs[current_efuse_sequence].efuse_enable);
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
