#include "app_pumpControl.hpp"
#include "io_time.hpp"
#include "io_loadswitches.hpp"
#include "app_canTx.hpp"

namespace app::pumpControl
{
consteval uint8_t SLOPE          = 1 / 2; // if specs have not changed
consteval uint8_t CURRENT_THRESH = 1 / 40;

static bool     finished_ramp_up = false;
static uint16_t time             = 0;

static void rampUp()
{
    // if we are done ramping up note that we are not ramping up
    if (finished_ramp_up)
    {
        return;
    }
    // calculate percentage based on defined slope above
    uint32_t percentage = SLOPE * time;
    app::can_tx::VC_PumpRampUpSetPoint_set(percentage);

    // should we add any buffer? is this seriously perfectly 100??
    if (percentage == 100)
    {
        time             = 0;
        finished_ramp_up = true;
    }
}

static void stopFlow()
{
    app::can_tx::VC_PumpFailure_set(true);
    finished_ramp_up = false;
    time             = 0;
}

void MonitorPumps()
{
    time += 10;
    // refactor required
    const bool pumps_ok = io::loadswitches::TILoadswitch_pgood(io::loadswitches::RL_PUMP_Efuse);

    const bool pumps_enabled = io::loadswitches::isChannelEnabled(io::TPS25_EfuseChannel::RL_PUMP);

    bool ramp_up_pumps = pumps_ok && pumps_enabled;

    if (ramp_up_pumps)
        rampUp();
    else
        stopFlow();

    app::can_tx::VC_RsmTurnOnPump_set(ramp_up_pumps);
}
} // namespace app::pumpControl
