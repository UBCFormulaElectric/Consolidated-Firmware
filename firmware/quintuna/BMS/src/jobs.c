#include "jobs.h"

#include "app_shdnLoop.h"
#include "app_heartbeatMonitors.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "io_bootHandler.h"
#include "io_canTx.h"

#include "io_canRx.h"
// app
#include "app_commitInfo.h"
#include "app_stateMachine.h"
#include "states/app_states.h"
// io
#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "io_canMsg.h"
#include "io_time.h"

CanTxQueue can_tx_queue;

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
}

static void charger_transmit_func(const JsonCanMsg *msg)
{
    // LOG_INFO("Send charger message: %d", msg->std_id);
}

void jobs_init()
{
    io_canTx_init(jsoncan_transmit_func, charger_transmit_func);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_charger(CHARGER_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    app_heartbeatMonitor_init(&hb_monitor);

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);

    app_stateMachine_init(&init_state);
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);
    app_shdnLoop_broadcast();

    app_stateMachine_tick100Hz();
    app_stateMachine_tickTransitionState();

    app_shdnLoop_broadcast();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}
