#include "app_pumpControl.hpp"
#include "io_time.hpp"
#include "io_efuses.hpp"
#include "io_pumpControl.hpp"
#include "io_semaphore.hpp"
#include "app_canTx.hpp"

#define BUFFER 5.0f

namespace
{
    io::semaphore i2c_bus { true };
}
namespace app::pumpControl
{
constexpr float SLOPE            = 1.0f / 2; // if specs have not changed
static bool     finished_ramp_up = false;
static uint16_t time_ms          = 0;

static float rampUpSetPoint(uint16_t current_time_ms)
{
    const float percentage = SLOPE * static_cast<float>(current_time_ms);
    app::can_tx::VC_PumpRampUpSetPoint_set(percentage);
    return percentage;
}

void MonitorPumps()
{
    const io::unique_semaphore lock{ i2c_bus };

    time_ms += 10;

    const auto rr_ready = rr_pump.isReady();

    if (rr_ready)
    {
        app::can_tx::VC_PumpFailure_set(false);
        if (!finished_ramp_up)
        {
            const float percentage = rampUpSetPoint(time_ms);
            if (percentage >= 100.0f - BUFFER)
            {
                time_ms          = 0;
                finished_ramp_up = true;
            }
        }
    }
    else
    {
        // stops the flow to reramp up to a setpoint
        app::can_tx::VC_PumpFailure_set(true);
        finished_ramp_up = false;
        time_ms          = 0;
    }
    app::can_tx::VC_RsmTurnOnPump_set(rr_ready);
}

#ifdef TARGET_TEST
void reset()
{
    finished_ramp_up = false;
    time_ms          = 0;
    app::can_tx::VC_PumpFailure_set(false);
    app::can_tx::VC_RsmTurnOnPump_set(false);
    app::can_tx::VC_PumpRampUpSetPoint_set(0.0f);
}
#endif
} // namespace app::pumpControl
