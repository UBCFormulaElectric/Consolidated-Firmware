#include "jobs.h"

#include "app_precharge.h"
#include "app_segments.h"
#include "app_shdnLoop.h"
#include "app_heartbeatMonitors.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "io_bootHandler.h"
#include "io_canTx.h"

#include "io_canTx.h"
#include "io_canRx.h"
// app
#include "app_commitInfo.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "app_stateMachine.h"
// io
#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "app_stateMachine.h"
#include "io_canMsg.h"
#include "io_time.h"
#include "io_log.h"

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

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);

    app_precharge_init();
    // app_heartbeatMonitor_init(&hb_monitor);

    app_segments_setDefaultConfig();
    app_segments_initFaults();
    app_segments_balancingInit();

    app_allStates_init();
    app_stateMachine_init(app_initState_get());
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg       = io_canQueue_popRx();
    JsonCanMsg   json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&json_can_msg);
}
