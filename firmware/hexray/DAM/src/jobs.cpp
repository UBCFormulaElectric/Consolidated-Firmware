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
                const auto epoch_ms = app::epochClock::getEpochMs();
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
    io::can_tx::DAM_Bootup_sendAperiodic();
    app::epochClock::logDateTime("Boot RTC time (GMT)");

    app::tsim::init();
    app::buzzer::init();
}

void jobs_initLogFs()
{
    // std::array<uint8_t, 512> wblk0{};
    // wblk0.fill(0xff);
    // wblk0[510] = 0x55;
    // wblk0[511] = 0xAA;
    // LOG_IF_ERR(sd1.write(wblk0, 16));
    // LOG_INFO("write done");

    // // --- Raw single-block read test of block 0 ---
    // while (1)
    // {
    //     static uint8_t block0[512] __attribute__((aligned(4)));
    //     LOG_INFO("attempting read");
    //     if (const auto res = sd1.read(std::span(block0, 512), 16))
    //     {
    //         LOG_INFO(
    //             "blk0 first 16: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
    //             block0[0], block0[1], block0[2], block0[3], block0[4], block0[5], block0[6], block0[7], block0[8],
    //             block0[9], block0[10], block0[11], block0[12], block0[13], block0[14], block0[15]);
    //         LOG_INFO("blk0 sig (should be 55 AA): %02X %02X", block0[510], block0[511]);
    //         break;
    //     }
    //     else
    //     {
    //         LOG_ERROR("SD Read Failed: %s", error_code_to_string(res.error()));
    //         LOG_INFO("Card state: %s", sd1.getCardStateString());
    //     }
    // }

    if (const auto err = app::sd::init_fs(); !err.has_value())
    {
        LOG_ERROR("jobs_initLogFs: init_fs failed: %d", static_cast<int>(err.error()));
        return;
    }

    if (const auto err = app::sd::update_metadata(); !err.has_value())
        LOG_ERROR("jobs_initLogFs: update_metadata failed: %d", static_cast<int>(err.error()));
}

void jobs_run1Hz_tick()
{
    if (app::sd::isLogOpen())
    {
        if (const auto err = fs.sync(app::sd::getLogFd()); !err)
        {
            LOG_ERROR("Log sync failed: %d", static_cast<int>(err.error()));
        }
    }
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
    const auto msg = log_queue.pop();
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
