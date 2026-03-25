#include "app_pumpControl.hpp"
#include "io_time.hpp"
#include "io_loadswitches.hpp"
#include "app_canTx.hpp"

#define BUFFER 5

namespace app::pumpControl
{
constexpr uint8_t SLOPE            = 1 / 2; // if specs have not changed
constexpr uint8_t CURRENT_THRESH   = 1 / 40;
static bool       finished_ramp_up = false;
static uint16_t   time_ms          = 0;

static void rampUp()
{
    if (finished_ramp_up)
    {
        return;
    }

    const float percentage = SLOPE * static_cast<float>(time_ms);
    app::can_tx::VC_PumpRampUpSetPoint_set(percentage);

    if (percentage >= 100.0f - BUFFER)
    {
        time_ms          = 0;
        finished_ramp_up = true;
    }
}

static void stopFlow()
{
    app::can_tx::VC_PumpFailure_set(true);
    finished_ramp_up = false;
    time_ms          = 0;
}

void MonitorPumps()
{
    time_ms += 10;

    const auto rl_ready = rl_pump.isReady();
    const auto rr_ready = rr_pump.isReady();

    const bool ramp_up_rl_pump = rl_ready && *rl_ready;
    const bool ramp_up_rr_pump = rr_ready && *rr_ready;

    if (ramp_up_rl_pump && ramp_up_rr_pump)
    {
        app::can_tx::VC_PumpFailure_set(false);
        rampUp();
    }
    else
    {
        stopFlow();
    }

    app::can_tx::VC_RsmTurnOnPump_set(ramp_up_rl_pump && ramp_up_rr_pump);
}
} // namespace app::pumpControl
