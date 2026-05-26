#include "app_buzzer.hpp"

#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_timer.hpp"
#include "io_buzzer.hpp"

namespace app::buzzer
{
namespace
{
    constexpr uint32_t PULSE_DURATION_MS = 1000;
    app::Timer         pulse_timer(PULSE_DURATION_MS);
} // namespace

void init()
{
    // Timer constructs in EXPIRED on embedded. Move it to IDLE so the first rising
    // edge of vc_drive_state arms the pulse, and make sure the buzzer starts off.
    pulse_timer.stop();
    io::buzzer::disable();
}

void tick()
{
    const bool vc_drive_state = app::can_rx::VC_State_get() == app::can_utils::VCState::VC_DRIVE_STATE;

    switch (pulse_timer.runIfCondition(vc_drive_state))
    {
        case app::Timer::TimerState::RUNNING:
            io::buzzer::enable();
            break;
        case app::Timer::TimerState::IDLE:
        case app::Timer::TimerState::EXPIRED:
        default:
            io::buzzer::disable();
            break;
    }
}

} // namespace app::buzzer
