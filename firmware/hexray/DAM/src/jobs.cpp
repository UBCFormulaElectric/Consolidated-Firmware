#include "jobs.hpp"
#include "app_ntp.hpp"
#include "app_button.hpp"
#include "io_log.hpp"

#include "app_bootcount.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_heartbeatMonitors.hpp"

#include "io_canMsg.hpp"
#include "io_canQueues.hpp"
#include "io_canTx.hpp"
#include "io_queue.hpp"
#include "io_telemMessage.hpp"
#include "io_telemQueue.hpp"
#include "io_logQueue.hpp"
#include "io_time.hpp"
#include "io_fileSystems.hpp"
#include "hw_sds.hpp"
#include "hw_gpios.hpp"
#include "main.h"

#include "util_errorCodes.hpp"

#include <span>
// priv namespace for the logfs vars
namespace
{
auto     LOG_PATH = "/log.bin";
uint32_t log_fd   = 0;
bool     log_open = false;
} // namespace

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            if (auto result = can_tx_queue.push(msg); not result)
                LOG_ERROR("Failed to push TX CAN message: %d", static_cast<int>(result.error()));
            (void)telem_tx_queue.push(io::telemMessage::TelemCanMsg(msg, io::time::getCurrentMs()));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    telem_tx_queue.init();
    log_queue.init();

    app::can_tx::DAM_Heartbeat_set(true);
}

void jobs_initLogFs()
{
    LOG_INFO("hsd1 state: %s", sd1.getCardStateString());
    LOG_IF_ERR(sd1.upgrade_buswidth());
    LOG_INFO("upgraded buswidth");
    LOG_IF_ERR(sd1.update_speed());
    LOG_INFO("upgraded speed");

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

    if (const auto err = fs.init(); not err)
    {
        LOG_ERROR("Failed to init filesystem: %d", static_cast<int>(err.error()));
    }
    else
    {
        LOG_INFO("we init fine");
    }

    if (const auto r = fs.open(LOG_PATH); r)
    {
        log_fd   = r.value();
        log_open = true;
    }
    else
    {
        LOG_ERROR("Failed to open %s: %d", LOG_PATH, static_cast<int>(r.error()));
    }

    if (const auto err = app::bootcount::update(fs); !err)
    {
        LOG_ERROR("Failed to update bootcount: %d", static_cast<int>(err.error()));
    };
}
void jobs_run1Hz_tick()
{
    if (log_open)
    {
        if (const auto err = fs.sync(log_fd); !err)
        {
            LOG_ERROR("Log sync failed: %d", static_cast<int>(err.error()));
        }
    }
}
void jobs_run100Hz_tick()
{
    if (app::button::ntpWasJustPressed())
    {
        if (const auto push_result = telem_tx_queue.push(io::telemMessage::NTPMsg{}); !push_result)
        {
            LOG_ERROR("Failed to enqueue NTP message: %d", static_cast<int>(push_result.error()));
        }
        if (const auto log_result = log_queue.push(io::telemMessage::NTPMsg{}); !log_result)
        {
            LOG_ERROR("Failed to log NTP message: %d", static_cast<int>(log_result.error()));
        }
    }
    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    // io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    // io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runLogging_tick()
{
    const auto msg = log_queue.pop();
    if (!msg || !log_open)
        return;

    // Serialize exactly like tasks_runTelemTx does for UART, so the SD log is
    // byte-identical to the telem wire stream.
    const auto wire = std::visit([](const auto &m) { return m.asBytes(); }, msg.value());
    if (const auto err = fs.write(log_fd, { const_cast<uint8_t *>(wire.data()), wire.size() }, wire.size()); !err)
    {
        LOG_ERROR("Log write failed: %d", static_cast<int>(err.error()));
    }
    else
    {
        // comment this out later
        LOG_INFO("Logged message of %u bytes", wire.size());
    }
}

void jobs_runTelem_tick() {}

void jobs_runTelemRx() {}
