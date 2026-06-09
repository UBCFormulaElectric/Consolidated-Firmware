#include "app_pumpControl.hpp"
#include "app_canTx.hpp"
#include "app_timer.hpp"
#include "io_semaphores.hpp"
#include "io_efuses.hpp"
#include "io_pumps.hpp"

constexpr float BUFFER = 50.0f;

namespace
{
}
namespace app::pumpControl
{
static constexpr uint32_t RAMP_DURATION_MS = 2000;
static app::Timer         ramp_timer_rl{ RAMP_DURATION_MS };
static app::Timer         ramp_timer_rr{ RAMP_DURATION_MS };
static constexpr uint8_t  MAX_PUMP_VALUE = 50;

void restart()
{
    ramp_timer_rl.stop();
    ramp_timer_rr.stop();
}

void MonitorPumps()
{
    if (rl_pump_efuse.isChannelEnabled() and rl_pump_efuse.ok())
    {
        switch (ramp_timer_rl.updateAndGetState())
        {
            case Timer::TimerState::IDLE:
                ramp_timer_rl.restart();
                break;
            case Timer::TimerState::RUNNING:
            case Timer::TimerState::EXPIRED:
            default:
                break;
        }
        const float percentage =
            static_cast<float>(ramp_timer_rl.getElapsedTime()) / static_cast<float>(RAMP_DURATION_MS);
        const auto percentage_u8 = static_cast<uint8_t>(percentage * MAX_PUMP_VALUE);
        app::can_tx::VC_RLPumpSetpoint_set(percentage_u8);
    }
    else
    {
        // stops the flow to reramp up to a setpoint
        ramp_timer_rl.stop();
        app::can_tx::VC_RLPumpSetpoint_set(0);
    }

    if (rr_pump_efuse.isChannelEnabled() and rr_pump_efuse.ok())
    {
        switch (ramp_timer_rr.updateAndGetState())
        {
            case Timer::TimerState::IDLE:
                ramp_timer_rr.restart();
                break;
            case Timer::TimerState::RUNNING:
            case Timer::TimerState::EXPIRED:
            default:
                break;
        }
        {
            const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
            const float                percentage =
                static_cast<float>(ramp_timer_rr.getElapsedTime()) / static_cast<float>(RAMP_DURATION_MS);
            const auto percentage_u8 = static_cast<uint8_t>(percentage * MAX_PUMP_VALUE);
            LOG_IF_ERR(rr_pump.setPercentage(percentage_u8));
            app::can_tx::VC_RRPumpSetpoint_set(percentage_u8);
        }
    }
    else
    {
        // stops the flow to reramp up to a setpoint
        ramp_timer_rr.stop();
        {
            const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
            LOG_IF_ERR(rr_pump.setPercentage(0));
            app::can_tx::VC_RRPumpSetpoint_set(0);
        }
    }
}

#ifdef TARGET_TEST
void reset()
{
    // finished_ramp_up = false;
    // time_ms          = 0;
    app::can_tx::VC_PumpFailure_set(false);
    app::can_tx::VC_RsmTurnOnPump_set(false);
    app::can_tx::VC_PumpRampUpSetPoint_set(0.0f);
}
#endif
} // namespace app::pumpControl
