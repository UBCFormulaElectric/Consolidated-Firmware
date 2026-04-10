#include "jobs.hpp"

#include <app_canUtils.hpp>
#include "app_jsoncan.hpp"

#include "io_canMsg.hpp"
#include <io_canTx.hpp>
#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_batteryMonitoring.hpp"

#include <util_errorCodes.hpp>

void jobs_init()
{
    fdcan_tx_queue.init();
    invcan_tx_queue.init();
    can_rx_queue.init();

    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(fdcan_tx_queue.push(msg));
        },
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(invcan_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    io::can_tx::enableMode_InvCAN(app::can_utils::InvCANMode::INVCAN_MODE_DEFAULT, true);
}
void jobs_run1Hz_tick()
{
    /*CellNum arr[5] = { CellNum::CELL1, CellNum::CELL2, CellNum::CELL3, CellNum::CELL4, CellNum::CELL5 };
    for (auto cell : arr)
    {
        auto v = io::batteryMonitoring::get_voltage(cell);
        if (v.has_value())
            LOG_INFO("Cell %u: %u mv", static_cast<int>(cell), v.value());
        else
            LOG_ERROR("Cell %u: Read failed", static_cast<int>(cell));
    } */
    io::batteryMonitoring::tick();
}
void jobs_run100Hz_tick()
{
    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
