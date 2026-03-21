#include "app_pumpControl.hpp"
#include "io_time.hpp"
#include "io_loadswitches.hpp"
#include "app_canTx.hpp"
#define BUFFER 5
namespace app::pumpControl
{
constexpr uint8_t SLOPE          = 1 / 2; // if specs have not changed
constexpr uint8_t CURRENT_THRESH = 1 / 40;
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
    float percentage = SLOPE * time;
    app::can_tx::VC_PumpRampUpSetPoint_set(percentage);

    // should we add any buffer? is this seriously perfectly 100??
    if (percentage >= 100 - BUFFER)
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
    const bool pump_rl_ok = RL_PUMP_Efuse.ok();
    const bool pump_rl_enabled = RL_PUMP_Efuse.isChannelEnabled();
    const bool pump_rr_ok = RR_PUMP_Efuse.ok();
    const bool pump_rr_enabled = RR_PUMP_Efuse.isChannelEnabled();

    bool ramp_up_rl_pump = pump_rl_ok && pump_rl_enabled;
    bool ramp_up_rr_pump = pump_rr_ok && pump_rr_enabled;

    if (ramp_up_rl_pump && ramp_up_rr_pump)
        rampUp();
    else
        stopFlow();

    app::can_tx::VC_RsmTurnOnPump_set(ramp_up_rl_pump && ramp_up_rr_pump);
}
} // namespace app::pumpControl
