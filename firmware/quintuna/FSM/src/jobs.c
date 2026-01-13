#include "jobs.h"
// app
#include "app_commitInfo.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_jsoncan.h"
#include "app_imu.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_steering.h"
#include "app_shdnLoop.h"
#include "app_suspension.h"
#include "app_utils.h"
// io
#include "io_time.h"
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_imu.h"
#include "io_log.h"

CanTxQueue can_tx_queue;

static void canTransmit(const JsonCanMsg *msg)
{
    const CanMsg m = app_jsoncan_copyToCanMsg(msg);
    io_canQueue_pushTx(&can_tx_queue, &m);
}

void jobs_init(void)
{
    // can
    io_canTx_init(canTransmit);
    io_canTx_enableMode_can2(CAN2_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);
    app_canTx_init();
    app_canRx_init();
    LOG_IF_ERR(io_imu_init());
    app_apps_init();

    app_canTx_FSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_FSM_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_FSM_Heartbeat_set(true);
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    app_apps_broadcast();
    app_brake_broadcast();
    app_steering_broadcast();
    app_shdnLoop_broadcast();
    app_suspension_broadcast();
    app_imu_broadcast();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enableMode_can2(CAN2_MODE_DEBUG, app_canRx_Debug_EnableDebugMode_get());
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}
