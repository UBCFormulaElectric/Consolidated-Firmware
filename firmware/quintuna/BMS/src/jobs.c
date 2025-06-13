#include "jobs.h"

// app
#include "states/app_states.h"
#include "app_precharge.h"
#include "app_segments.h"
#include "app_heartbeatMonitors.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_imd.h"
#include "app_commitInfo.h"
#include "states/app_allStates.h"
#include "app_jsoncan.h"
#include "app_tractiveSystem.h"
#include "app_irs.h"
#include "app_shdnLoop.h"
// io
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_canMsg.h"
#include "io_time.h"
#include "io_bspdTest.h"
#include "io_charger.h"
#include "io_faultLatch.h"
#include "io_irs.h"

#include <app_canAlerts.h>

CanTxQueue can_tx_queue; // TODO there HAS to be a better location for this

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
}

static void charger_transmit_func(const JsonCanMsg *msg)
{
    // LOG_INFO("Send charger message: %d", msg->std_id);
    UNUSED(msg);
}

void jobs_init()
{
    io_canTx_init(jsoncan_transmit_func, charger_transmit_func);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_charger(CHARGER_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    app_precharge_init();
    // app_heartbeatMonitor_init(&hb_monitor);

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);

    app_segments_initFaults();

    app_allStates_init();
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

    app_tractiveSystem_broadcast();
    app_imd_broadcast();
    app_irs_broadcast();
    app_shdnLoop_broadcast();

    io_bspdTest_enable(app_canRx_Debug_EnableTestCurrent_get());
    app_canTx_BMS_BSPDCurrentThresholdExceeded_set(io_bspdTest_isCurrentThresholdExceeded());

    // If charge state has not placed a lock on broadcasting
    // if the charger is charger is connected
    app_canTx_BMS_ChargerConnectedType_set(io_charger_getConnectionStatus());

    (void)app_segments_checkWarnings();
    const bool ams_fault = app_segments_checkFaults();

    // Update CAN signals for BMS latch statuses.
    io_faultLatch_setCurrentStatus(&bms_ok_latch, ams_fault);
    app_canTx_BMS_BmsCurrentlyOk_set(io_faultLatch_getCurrentStatus(&bms_ok_latch));
    app_canTx_BMS_ImdCurrentlyOk_set(io_faultLatch_getCurrentStatus(&imd_ok_latch));
    app_canTx_BMS_BspdCurrentlyOk_set(io_faultLatch_getCurrentStatus(&bspd_ok_latch));
    app_canTx_BMS_BmsLatchOk_set(io_faultLatch_getLatchedStatus(&bms_ok_latch));
    app_canTx_BMS_ImdLatchOk_set(io_faultLatch_getLatchedStatus(&imd_ok_latch));
    app_canTx_BMS_BspdLatchOk_set(io_faultLatch_getLatchedStatus(&bspd_ok_latch));

    app_canAlerts_BMS_Fault_AMSFault_set(ams_fault);
    app_canAlerts_BMS_Fault_IMDFault_set(false);  // TODO
    app_canAlerts_BMS_Fault_BSPDFault_set(false); // TODO

    app_stateMachine_tick100Hz();

    if (io_irs_negativeState() == IRS_OPEN)
    {
        app_stateMachine_setNextState(&init_state);
    }
    if (app_canAlerts_AnyBoardHasFault())
    {
        app_stateMachine_setNextState(&fault_state);
    }
    app_stateMachine_tickTransitionState();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}
