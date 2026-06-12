#include "jobs.hpp"
#include "app_ntp.hpp"
#include "app_button.hpp"
#include "io_log.hpp"

#include "app_bootcount.hpp"
#include "app_buzzer.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_canDataCapture.hpp"
#include "app_jsoncan.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_commitInfo.hpp"
#include "app_epochClock.hpp"
#include "app_sd.hpp"
#include "app_tsim.hpp"
#include "app_damShdnLoop.hpp"

#include "io_canMsg.hpp"
#include "io_canQueues.hpp"
#include "io_canTx.hpp"
#include "io_queue.hpp"
#include "io_telemMessage.hpp"
#include "io_telemQueue.hpp"
#include "io_logQueue.hpp"
#include "io_time.hpp"
#include "io_fileSystems.hpp"
#include "io_sd.hpp"

#include "io_canLogging.hpp"
#include "util_errorCodes.hpp"

#include <span>

void jobs_init()
{
    telem_tx_queue.init();
    log_queue.init();
    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg    = app::jsoncan::copyToCanMsg(tx_msg);
            const uint32_t   now_ms = io::time::getCurrentMs();
            if (auto result = can_tx_queue.push(msg); not result)
                LOG_ERROR("Failed to push TX CAN message: %d", static_cast<int>(result.error()));
            // Mirror DAM bootup TX into SD logging path.
            if (tx_msg.std_id == app::can_utils::DAM_Bootup_Signals::MSG_ID)
            {
                (void)log_queue.push(msg);
            }
            if (app::can_data_capture::needsTelem(msg.std_id, now_ms))
            {
                const auto epoch_ms = app::epochClock::getEpochMsFast();
                if (epoch_ms)
                {
                    (void)telem_tx_queue.push(io::telemMessage::TelemCanMsg(msg, *epoch_ms));
                }
                else
                {
                    LOG_WARN(
                        "telem TX timestamp unavailable, dropping CAN 0x%03lX", static_cast<unsigned long>(msg.std_id));
                }
            }
        });

    app::can_tx::DAM_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::DAM_Clean_set(GIT_COMMIT_CLEAN);
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    app::can_tx::DAM_Heartbeat_set(true);
    app::can_tx::DAM_Alive_set(1);
    // Anchor the fast clock once here, before the scheduler and any reader; setEpochMs() re-anchors
    // it on NTP correction.
    if (const auto err = app::epochClock::anchorBaseTime(); !err.has_value())
    {
        LOG_WARN("jobs_init: anchorBaseTime failed: %d", static_cast<int>(err.error()));
    }
    io::can_tx::DAM_Bootup_sendAperiodic();
    app::epochClock::logDateTime("Boot RTC time (GMT)");

    app::tsim::init();
    app::buzzer::init();
}

void jobs_initLogFs()
{
    bool init_success = true;

    if (const auto err = app::sd::init_fs(); !err.has_value())
    {
        LOG_ERROR("jobs_initLogFs: init_fs failed: %d", static_cast<int>(err.error()));
        init_success = false;
    }

    // Base was already anchored in jobs_init(); the metadata write just reads it via getFastBase().
    app::sd::requestMetadataUpdate();
    LOG_INFO("jobs_initLogFs: Metadata update requested in init");

    if (const auto err = io::sd::upgrade(); !err.has_value())
    {
        LOG_ERROR("jobs_initLogFs: io::sd::upgrade failed: %d", static_cast<int>(err.error()));
        init_success = false;
    }

    if (init_success)
        LOG_INFO("Filesystem initialized successfully");
}

void jobs_run1Hz_tick()
{
    // DEBUG: re-send the bootup frame once, ~5s after boot, to test whether a delayed
    // copy survives the telem path when the boot-time copy (sent in jobs_init) doesn't.
    static uint32_t seconds_since_boot = 0;
    if (++seconds_since_boot == 5U)
    {
        LOG_INFO("debug: re-sending DAM_Bootup at t=%lus", static_cast<unsigned long>(seconds_since_boot));
        io::can_tx::DAM_Bootup_sendAperiodic();
    }

    {
        static uint32_t last_telem_tx_overflow = 0;
        const uint32_t  overflow                 = telem_tx_queue.get_overflowCount();
        if (overflow != last_telem_tx_overflow)
        {
            LOG_WARN(
                "telem_tx_queue overflow count: %lu (+%lu)",
                static_cast<unsigned long>(overflow),
                static_cast<unsigned long>(overflow - last_telem_tx_overflow));
            last_telem_tx_overflow = overflow;
        }
    }

    app::sd::requestSync();
    io::can_tx::enqueue1HzMsgs();
}
void jobs_run100Hz_tick()
{
    if (app::button::ntpWasJustPressed())
    {
        if (const auto push_result = telem_tx_queue.push(io::telemMessage::NTPMsg{}); !push_result)
        {
            LOG_ERROR("Failed to enqueue NTP message: %d", static_cast<int>(push_result.error()));
        }
        // NTP markers go over UART only; the SD log is raw CAN frames only.
    }

    app::buzzer::tick();

    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    dam_shdnLoop.broadcast();

    io::can_tx::enqueue100HzMsgs();

    app::tsim::tick();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runLogging_tick()
{
    constexpr uint32_t logging_poll_ms = 100U;

    const auto msg = log_queue.pop(logging_poll_ms);
    app::sd::service();
    if (!msg || !app::sd::isLogOpen())
        return;

    io::canLogging::EncodeBuf buf;
    const size_t              n = io::canLogging::encode(msg.value(), buf);

    if (const auto err = fs.write(app::sd::getLogFd(), { buf.data(), n }, n); !err)
    {
        LOG_ERROR("Log write failed: %d", static_cast<int>(err.error()));
    }
}

void jobs_runTelem_tick() {}

void jobs_runTelemRx() {}
