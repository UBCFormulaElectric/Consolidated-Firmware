#include "jobs.hpp"

#include "app_canTx.hpp"
#include "app_commitInfo.h"
#include "screens/app_screens.hpp"

#include "io_canTx.hpp"
#include "io_switches.hpp"
#include "io_time.hpp"
#include "io_leds.hpp"
#include "io_powerGauge.hpp"

void jobs_init()
{
    app::can_tx::CRIT_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::CRIT_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::CRIT_Heartbeat_set(true);

    app::screens::init();
}

void jobs_run1Hz_tick()
{
    io::can_tx::enqueue1HzMsgs();
}
void jobs_run100Hz_tick()
{
    // TODO move this to app_leds
    io::leds::update({
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        false,
        false,
        false,
        false,
        false,
    });
    io::leds::setBrightness(1.0);

    app::screens::tick();

    // io::power_gauge::update({});

    // update the state from the switches
    app::can_tx::CRIT_TorqueVectoringSwitch_set(io::switches::torque_vectoring_get());
    app::can_tx::CRIT_LaunchControlSwitch_set(io::switches::launch_control_get());
    app::can_tx::CRIT_RegenSwitch_set(io::switches::regen_get());
    app::can_tx::CRIT_StartButton_set(io::switches::start_get());
    // TODO debounce and find rising edge
    // if (const bool has_rising_edge = io::switches::telem_mark_get(); has_rising_edge)
    // {
    //     io::can_tx::CRIT_TelemMarkEvent_sendAperiodic();
    // }

    // enqueue can messages
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
