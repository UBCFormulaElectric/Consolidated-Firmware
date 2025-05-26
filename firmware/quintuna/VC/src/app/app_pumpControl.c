#include "app_pumpControl.h"
#include "io_loadswitch.h"
#include "io_time.h"
#include "io_loadswitches.h"
#include <app_canTx.h>

#define SLOPE (0.5f)

static bool     finished_ramp_up = false;
static uint16_t time             = 0;

#define CURRENT_THRESH 0.025f

static bool TILoadswitch_pgood(const TI_LoadSwitch *loadSwitch)
{
    assert(loadSwitch->pgood != NULL);
    return hw_gpio_readPin(loadSwitch->pgood);
}

static void pumpControl_rampUp(void)
{
    // if we are done ramping up note that we are not ramping up
    if (finished_ramp_up)
    {
        return;
    }
    // calcualte percentage based on defined slope above
    uint8_t percentage = (uint8_t)(SLOPE * time);

    io_pumpControl_setPercentage(percentage, RR_PUMP);
    io_pumpControl_setPercentage(percentage, F_PUMP);
    app_canTx_VC_PumpRampUpSetPoint_set((uint32_t)percentage);

    if (percentage == 100)
    {
        time             = 0;
        finished_ramp_up = true;
    }
}

static void pumpControl_stopFlow(void)
{
    io_pumpControl_setPercentage(0, RR_PUMP);
    io_pumpControl_setPercentage(0, F_PUMP);

    app_canTx_VC_PumpFailure_set(true);
    finished_ramp_up = false;
    time             = 0;
}

void app_pumpControl_MonitorPumps(void)
{
    const bool pumps_ok = TILoadswitch_pgood(&f_pump_loadswitch) && TILoadswitch_pgood(&rl_pump_loadswitch) &&
                          TILoadswitch_pgood(&rr_pump_loadswitch);

    const bool pumps_enabled =
        (io_loadswitch_isChannelEnabled(efuse_channels[EFUSE_CHANNEL_RR_PUMP]) ||
         io_loadswitch_isChannelEnabled(efuse_channels[EFUSE_CHANNEL_RL_PUMP]) ||
         io_loadswitch_isChannelEnabled(efuse_channels[EFUSE_CHANNEL_RR_PUMP]));

    if (pumps_ok && pumps_enabled)
        pumpControl_rampUp();
    else
        pumpControl_stopFlow();
}