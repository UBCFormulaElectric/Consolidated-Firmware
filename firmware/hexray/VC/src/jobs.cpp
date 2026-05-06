#include "jobs.hpp"

#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_canRx.hpp"

#include "io_canMsg.hpp"
#include "io_canTx.hpp"
#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"

void jobs_init()
{
    fdcan_tx_queue.init();
    invcan_tx_queue.init();
    can_rx_queue.init();

    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            UNUSED(fdcan_tx_queue.push(msg));
        },
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            UNUSED(invcan_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_InvCAN(app::can_utils::InvCANMode::INVCAN_MODE_DEFAULT, true);

    // io::batteryMonitoring::init();
}
void jobs_run1Hz_tick()
{
    io::batteryMonitoring::random();
}
void jobs_run100Hz_tick()
{
    // io::can_tx::enqueue100HzMsgs();
    // const uint32_t k = app::can_rx::BMS_ChargePowerLimit_get();
    // LOG_INFO("%d", k);
    // hb_monitor.checkIn();
    // hb_monitor.broadcastFaults();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
