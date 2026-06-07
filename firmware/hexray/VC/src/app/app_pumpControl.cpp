#include "app_pumpControl.hpp"
#include "app_canTx.hpp"
#include "app_timer.hpp"

#include "io_efuses.hpp"
#include "io_pumpControl.hpp"
#include "io_semaphores.hpp"

constexpr float BUFFER = 50.0f;

namespace
{
}
namespace app::pumpControl
{
static constexpr uint32_t RAMP_DURATION_MS = 200;
static app::Timer         ramp_timer{ RAMP_DURATION_MS };

void MonitorPumps()
{
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    const auto                 rr_ready = rr_pump.isReady();

    if (rr_ready)
    {
        switch (ramp_timer.updateAndGetState())
        {
            case Timer::TimerState::IDLE:
                ramp_timer.restart();
                break;
            case Timer::TimerState::RUNNING:
            case Timer::TimerState::EXPIRED:
            default:
                break;
        }
        const float percentage = static_cast<float>(ramp_timer.getElapsedTime()) / static_cast<float>(RAMP_DURATION_MS);
        LOG_IF_ERR(rr_pump.setPercentage(static_cast<uint8_t>(percentage * 100)));
        app::can_tx::VC_PumpRampUpSetPoint_set(percentage);
    }
    else
    {
        // stops the flow to reramp up to a setpoint
        ramp_timer.stop();
        app::can_tx::VC_PumpRampUpSetPoint_set(0);
    }

    app::can_tx::VC_PumpFailure_set(not rr_ready);
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
