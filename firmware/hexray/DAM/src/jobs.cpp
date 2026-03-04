#include "jobs.hpp"

#include <app_canUtils.hpp>
#include "app_jsoncan.hpp"

#include "io_canMsg.hpp"
#include <io_canTx.hpp>
#include "io_canQueues.hpp"
#include "io_time.hpp"

#include <util_errorCodes.hpp>

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
void jobs_runTelem_tick() {}
void jobs_runTelemRx() {}