#include "jobs.hpp"

#include "app_canTx.hpp"
#include "app_commitInfo.h"
#include "app_jsoncan.hpp"
#include "screens/app_screens.hpp"
#include "app_heartbeatMonitors.hpp"

#include "io_canTx.hpp"
#include "io_switches.hpp"
#include "io_time.hpp"
#include "io_leds.hpp"
#include "io_powerGauge.hpp"
#include "io_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_math.hpp"

#ifdef TARGET_EMBEDDED
#include "hw_pwmOutputs.hpp"
#endif

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(can_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);

    app::can_tx::CRIT_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::CRIT_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::CRIT_Heartbeat_set(true);

#ifdef TARGET_EMBEDDED
    LOG_IF_ERR(led_dimming.start());
    LOG_IF_ERR(led_dimming.setDutyCycle(100));
#endif

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

    static float angle = 0.0f;

    auto ccos_result = io::math::csin(angle);
    app::can_tx::CRIT_Cordic_Cos_set(ccos_result.value_or(0.0f));
    float libc_cos_result = std::sinf(angle);
    app::can_tx::CRIT_stdlib_Cos_set(libc_cos_result);
    float cos_diff = std::abs(ccos_result.value_or(0.0f) - libc_cos_result);

    angle += M_PI_F / 180.0f;    

    static float atan_input = -2.0f;

    auto atan_result = io::math::atan(atan_input);
    app::can_tx::CRIT_Cordic_Atan_set(atan_result.value_or(0.0f));
    app::can_tx::CRIT_stdlib_Atan_set(std::atanf(atan_input));

    atan_input += 0.01f;
    if (atan_input > 2.0f)
    {
        atan_input = -2.0f;
    }

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

    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    // enqueue can messages
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
