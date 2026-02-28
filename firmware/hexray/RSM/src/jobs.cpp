#include "jobs.hpp"

#include "app_brake.hpp"
#include "app_coolant.hpp"
#include "app_imu.hpp"
#include "app_jsoncan.hpp"
#include "app_suspension.hpp"
#include "app_tireTemp.hpp"
#include <app_canUtils.hpp>

#include "io_canQueues.hpp"
#include "io_imus.hpp"
#include "io_time.hpp"
#include "io_canMsg.hpp"
#include <io_canRx.hpp>
#include <io_canTx.hpp>

void jobs_init()
{
    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(can_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);
    app::imu::init();
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    app::brake::broadcast();
    app::imu::broadcast();
    app::suspension::broadcast();
    app::tireTemp::broadcast();

    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
