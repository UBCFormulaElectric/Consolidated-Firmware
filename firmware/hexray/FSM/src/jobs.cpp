#include "jobs.hpp"

#include "app_apps.hpp"
#include "app_brake.hpp"
#include "app_imu.hpp"
#include "app_jsoncan.hpp"
#include "app_steering.hpp"
#include "app_suspension.hpp"
#include "app_canUtils.hpp"
#include "app_canTx.hpp"
#include "app_heartbeatMonitors.hpp"
#include "app_commitInfo.hpp"
#include "app_fsmShdnLoop.hpp"

#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_canMsg.hpp"
#include "io_canTx.hpp"

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

    app::can_tx::FSM_Hash_set(GIT_COMMIT_HASH);
    app::can_tx::FSM_Clean_set(GIT_COMMIT_CLEAN);
    app::can_tx::FSM_Heartbeat_set(true);
    io::can_tx::FSM_Bootup_sendAperiodic();
}
void jobs_initImu()
{
    app::imu::init();
}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    app::apps::broadcast();
    app::brake::broadcast();
    app::imu::broadcast();
    // app::shdnLoop::broadcast();
    app::steering::broadcast();
    app::suspension::broadcast();
    fsm_shdnLoop.broadcast();

    hb_monitor.checkIn();
    hb_monitor.broadcastFaults();

    io::can_tx::enqueue100HzMsgs();
}
void jobs_run1kHz_tick()
{
    io::can_tx::enqueueOtherPeriodicMsgs(io::time::getCurrentMs());
}
void jobs_runImu_tick()
{
    app::imu::broadcast();
}
