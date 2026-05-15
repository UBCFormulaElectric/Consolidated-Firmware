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
#include "io_log.hpp"
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
constexpr const char *LOG_PATH = "/log.bin";
uint32_t              log_fd   = 0;
bool                  log_open = false;
} // namespace

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg    = app::jsoncan::copyToCanMsg(tx_msg);
            auto             result = can_tx_queue.push(msg);
            if (not result)
                LOG_ERROR("Failed to push TX CAN message: %d", static_cast<int>(result.error()));
            (void)telem_tx_queue.push(
                io::telemMessage::TelemCanMsg(msg, static_cast<uint64_t>(io::time::getCurrentMs())));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    telem_tx_queue.init();
    log_queue.init();

    app::can_tx::DAM_Heartbeat_set(true);
}

void jobs_initLogFs()
{
    const GPIO_PinState raw_cd = HAL_GPIO_ReadPin(SD_CD_GPIO_Port, SD_CD_Pin);
    LOG_INFO("SD_CD raw GPIO (PC7): %d (%s)", (int)raw_cd, raw_cd == GPIO_PIN_SET ? "HIGH" : "LOW");
    LOG_INFO("SD_CD via hw::gpio.readPin(): %d", (int)sd_present.readPin());
    LOG_INFO("sd present: %d", sd1.sdPresent());
    LOG_INFO("hsd1 state: %lu", (unsigned long)HAL_SD_GetCardState(&sd1.getHsd()));

    // --- Raw single-block read test of block 0 ---
    {
        static uint8_t          block0[512] __attribute__((aligned(4)));
        SD_HandleTypeDef       *hsd   = &sd1.getHsd();
        const uint32_t          t0    = HAL_GetTick();
        uint32_t                waits = 0;
        while (HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER && (HAL_GetTick() - t0) < 1000)
            waits++;
        const uint32_t          state_before = HAL_SD_GetCardState(hsd);
        const HAL_StatusTypeDef st           = HAL_SD_ReadBlocks(hsd, block0, 0, 1, 1000);
        const uint32_t          hal_err      = HAL_SD_GetError(hsd);
        const uint32_t          state_after  = HAL_SD_GetCardState(hsd);
        LOG_INFO(
            "raw read blk0: HAL_Status=%d state_before=%lu state_after=%lu HAL_SD_GetError=0x%08lX waits=%lu", (int)st,
            (unsigned long)state_before, (unsigned long)state_after, (unsigned long)hal_err, (unsigned long)waits);
        LOG_INFO(
            "blk0 first 16: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", block0[0],
            block0[1], block0[2], block0[3], block0[4], block0[5], block0[6], block0[7], block0[8], block0[9],
            block0[10], block0[11], block0[12], block0[13], block0[14], block0[15]);
        LOG_INFO("blk0 sig (should be 55 AA): %02X %02X", block0[510], block0[511]);
    }

    if (const auto err = fs.init(); !err)
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
        const auto push_result = telem_tx_queue.push(io::telemMessage::NTPMsg{});
        if (!push_result)
        {
            LOG_ERROR("Failed to enqueue NTP message: %d", static_cast<int>(push_result.error()));
        }
        const auto log_result = log_queue.push(io::telemMessage::NTPMsg{});
        if (!log_result)
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
    // byte-identical to the telem wire stream and the same backend parser decodes both.
    const auto         wire = std::visit([](const auto &m) { return m.asBytes(); }, msg.value());
    std::span<uint8_t> bytes{ const_cast<uint8_t *>(wire.data()), wire.size() };
    if (const auto err = fs.write(log_fd, bytes, wire.size()); !err)
    {
        LOG_ERROR("Log write failed: %d", static_cast<int>(err.error()));
    }
    else
    {
        LOG_INFO("Logged message of %zu bytes", wire.size());
    }
}

void jobs_runTelem_tick() {}

void jobs_runTelemRx() {}
