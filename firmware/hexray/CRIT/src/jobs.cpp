#include "jobs.hpp"

#include "app_canTx.hpp"
#include "io_canTx.hpp"
#include "io_switches.hpp"

void jobs_init() {}
void jobs_run1Hz_tick() {}
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
}
void jobs_run1kHz_tick() {}
void jobs_runCanTx_tick() {}
void jobs_runCanRx_tick() {}