#include "jobs.hpp"

#include "app_canTx.hpp"
#include "app_commitInfo.hpp"
#include "app_jsoncan.hpp"
#include "app_switches.hpp"
#include "app_leds.hpp"
#include "screens/app_screens.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_brightness.hpp"
#include "app_rgbhsv.hpp"

#include "io_canTx.hpp"
#include "io_time.hpp"
#include "io_canQueues.hpp"
#include "io_leds.hpp"
#include "io_powerGauge.hpp"
#include "io_sevenSeg.hpp"

#include <cmath>

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            UNUSED(tx_msg);
            // const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            // LOG_IF_ERR(can_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);

    app::can_tx::CRIT_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::CRIT_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::CRIT_Heartbeat_set(true);

    app::screens::init();
    app::leds::init();
}

void jobs_run1Hz_tick()
{
    io::can_tx::enqueue1HzMsgs();
}
void jobs_run100Hz_tick()
{
    app::leds::setLeds();
    app::screens::tick();

    const double h = std::fmod(static_cast<double>(io::time::getCurrentMs()) / 10, 360);
    auto [r, g, b] = app::rgb::from({ h, 1.0, 0.5 });
    const auto r_8 = static_cast<uint8_t>(r * static_cast<double>(std::numeric_limits<uint8_t>::max()));
    const auto g_8 = static_cast<uint8_t>(g * static_cast<double>(std::numeric_limits<uint8_t>::max()));
    const auto b_8 = static_cast<uint8_t>(b * static_cast<double>(std::numeric_limits<uint8_t>::max()));
    std::array<io::power_gauge::led_frame, io::power_gauge::LEDS> data{ {
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
        { static_cast<double>(app::brightness) / 100, r_8, g_8, b_8 },
    } };
    LOG_IF_ERR(io::power_gauge::update(data));

    app::switches::broadcast();
    // app::driveModes::broadcast();

    // TODO find rising edge
    // if (const bool has_rising_edge = io::switches::telem_mark_get(); has_rising_edge)
    // {
    //     io::can_tx::CRIT_TelemMarkEvent_sendAperiodic();
    // }

    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    LOG_IF_ERR(io::seven_seg::setBrightness(std::max(static_cast<uint8_t>(5u), app::brightness)));
    LOG_IF_ERR(io::leds::setBrightness(app::brightness));

    // enqueue can messages
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
