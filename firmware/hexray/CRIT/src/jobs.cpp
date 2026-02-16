#include "jobs.hpp"

#include "app_canTx.hpp"
#include "io_canTx.hpp"
#include "io_rotary.hpp"
#include "io_switches.hpp"
#include "io_time.hpp"
#include "app_canUtils.hpp"

static auto drive_mode = app::can_utils::DriveMode::DRIVE_MODE_POWER;

void jobs_init()
{
    io::rotary::init();
    io::rotary::setClockwiseCallback(
        []
        {
            drive_mode = static_cast<app::can_utils::DriveMode>(
                (static_cast<uint32_t>(drive_mode) + 1) %
                static_cast<uint32_t>(app::can_utils::DriveMode::DRIVE_MODE_COUNT));
        });
    io::rotary::setCounterClockwiseCallback(
        []
        {
            drive_mode = static_cast<app::can_utils::DriveMode>(
                (static_cast<uint32_t>(drive_mode) - 1) %
                static_cast<uint32_t>(app::can_utils::DriveMode::DRIVE_MODE_COUNT));
        });
    io::rotary::setPushCallback([] {});
}

void jobs_run1Hz_tick()
{
    io::can_tx::enqueue1HzMsgs();
}
void jobs_run100Hz_tick()
{
    // update the state from the switches
    app::can_tx::CRIT_TorqueVectoringSwitch_set(io::switches::torque_vectoring_get());
    app::can_tx::CRIT_LaunchControlSwitch_set(io::switches::launch_control_get());
    app::can_tx::CRIT_RegenSwitch_set(io::switches::regen_get());
    app::can_tx::CRIT_StartButton_set(io::switches::start_get());
    if (const bool has_rising_edge = io::switches::telem_mark_rising_edge_get(); has_rising_edge)
    {
        io::can_tx::sendAperiodic_CRIT_TelemMarkEvent();
    }

    // enqueue can messages
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
