#include "jobs.hpp"

#include "io_canQueues.hpp"
#include "io_log.hpp"
#include "io_telemMessage.hpp"
#include "io_queue.hpp"
#include "hw_uarts.hpp"
#include "io_telemQueue.hpp"
#include "app_jsoncan.hpp"
#include <app_canUtils.hpp>
#include "io_time.hpp"
#include "io_canMsg.hpp"
#include <io_canTx.hpp>
#include <util_errorCodes.hpp>

#include <span>

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
    telem_tx_queue.init();
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runLogging_tick() {}

void jobs_runTelem_tick()
{
    const auto result = telem_tx_queue.pop();
    if (not result)
    {
        LOG_ERROR("Failed to pop telem TX message: %d", static_cast<int>(result.error()));
        return;
    }

    const auto &msg = result.value();
    const auto  tx_result =
        _900k_uart.transmitPoll(std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&msg), msg.wireSize() });
    if (not tx_result)
    {
        LOG_ERROR("Failed to transmit telem message: %d", static_cast<int>(tx_result.error()));
    }
}

void jobs_runTelemRx()
{
    // TODO: NTP time-sync response — see quintuna DAM io_telemRx.c for protocol
    // Poll _900k_uart.receivePoll(...) for 0xFF 0x01 header, extract IoRtcTime, call io_rtc_setTime()
}
