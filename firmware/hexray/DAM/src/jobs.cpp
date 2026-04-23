#include "jobs.hpp"
#include "app_ntp.hpp"
#include "io_ntpButton.hpp"
#include "io_telemRx.hpp"
#include "io_log.hpp"

#include "io_canQueues.hpp"
#include "io_queue.hpp"
#include "io_telemQueue.hpp"
#include "app_jsoncan.hpp"
#include <app_canUtils.hpp>
#include "io_time.hpp"
#include "io_canMsg.hpp"
#include <io_canTx.hpp>

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg    = app::jsoncan::copyToCanMsg(tx_msg);
            auto             result = can_tx_queue.push(msg);
            // if (not result)
            // LOG_ERROR("Failed to push TX CAN message: %d", static_cast<int>(result.error()));
            (void)telem_tx_queue.push(
                io::telemMessage::TelemCanMsg(msg, static_cast<uint64_t>(io::time::getCurrentMs())));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    telem_tx_queue.init();
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    if (io::ntpButton::wasJustPressed())
    {
        const auto ntp_result = io::telemRx::transmitNTPStartMsg();
        if (!ntp_result)
        {
            LOG_ERROR("transmitNTPStartMsg() failed with error: %d", static_cast<int>(ntp_result.error()));
        }
        else
        {
            // TODO: enqueue to some ntp output queue, this will overwrite the t0 value
            app::ntp::recordT0(app::ntp::rtcTimeToMs(*ntp_result));
        }
    }
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runLogging_tick() {}

void jobs_runTelem_tick() {}

void jobs_runTelemRx() {}
