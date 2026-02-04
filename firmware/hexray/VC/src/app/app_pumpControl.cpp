#include "app_pumpControl.hpp"
extern "C"
{
#include "io_loadswitch.h"
#include "io_time.h"
#include "io_loadswitches.h"
#include <app_canTx.h>
}

namespace vc::app::pumpControl
{
constexpr float SLOPE          = 0.5f;
constexpr float CURRENT_THRESH = 0.025f;

static bool     finished_ramp_up = false;
static uint16_t time             = 0;

static void pumpControl_rampUp(void)
{
    // if we are done ramping up note that we are not ramping up
    if (finished_ramp_up)
    {
        return;
    }
    // calculate percentage based on defined slope above
    uint8_t percentage = (uint8_t)(SLOPE * time);
    app_canTx_VC_PumpRampUpSetPoint_set((uint32_t)percentage);

    if (percentage == 100)
    {
        time             = 0;
        finished_ramp_up = true;
    }
}

static void pumpControl_stopFlow(void)
{
    app_canTx_VC_PumpFailure_set(true);
    finished_ramp_up = false;
    time             = 0;
}

void app_pumpControl_MonitorPumps(void)
{
    time += 10;
    const bool pumps_ok = io_TILoadswitch_pgood(&rl_pump_loadswitch);

    const bool pumps_enabled = io_loadswitch_isChannelEnabled(EfuseChannel::TPS25_EfuseChannel::RL_PUMP);

    bool ramp_up_pumps = pumps_ok && pumps_enabled;

    if (ramp_up_pumps)
        pumpControl_rampUp();
    else
        pumpControl_stopFlow();

    app_canTx_VC_RsmTurnOnPump_set(ramp_up_pumps);
}
} // namespace vc::app::pumpControl
