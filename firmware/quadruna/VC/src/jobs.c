#include "jobs.h"

#include "app_canDataCapture.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_faultCheck.h"
#include "app_commitInfo.h"
#include "io_canTx.h"
#include "io_canRx.h"

#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "states/app_allStates.h"
#include "app_heartbeatMonitors.h"
#include "app_stackWaterMarks.h"

#include "io_time.h"
#include "io_log.h"
#include "io_sbgEllipse.h"
#include "io_imu.h"
#include "io_imu_config.h"
#include "io_jsoncan.h"
#include "io_canQueue.h"
#include "io_telemMessage.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "io_canMsg.h"
#include "io_bootHandler.h"

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);

    if (app_dataCapture_needsLog((uint16_t)msg.std_id, msg.timestamp))
    {
        io_canLogging_loggingQueuePush(&msg);
    }

    if (app_dataCapture_needsTelem((uint16_t)msg.std_id, msg.timestamp))
    {
        LOG_ERROR_IF(io_telemMessage_pushMsgtoQueue(&msg));
    }
}

void jobs_init()
{
    app_canTx_init();
    app_canRx_init();
    app_canDataCapture_init();

    // Empirically, mounting slows down (takes ~500ms) at 200 CAN logs on disk.
    // This is not correlated to the size of each file.
    app_canTx_VC_NumberOfCanDataLogs_set(io_canLogging_getCurrentLog());
    app_canAlerts_VC_Warning_HighNumberOfCanDataLogs_set(io_canLogging_getCurrentLog() > HIGH_NUMBER_OF_LOGS_THRESHOLD);
    app_canAlerts_VC_Warning_CanLoggingSdCardNotPresent_set(!io_fileSystem_present());

    app_stateMachine_init(app_initState_get());
    app_heartbeatMonitor_init(&hb_monitor);

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_VC_Heartbeat_set(true);

    app_faultCheck_init();

    if (!io_sbgEllipse_init())
    {
        app_canAlerts_VC_Warning_SbgInitFailed_set(true);
        LOG_INFO("Sbg initialization failed");
    }
    if (!io_imu_init())
    {
        app_canAlerts_VC_Warning_ImuInitFailed_set(true);
        LOG_INFO("Imu initialization failed");
    }

    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_canQueue_init();
    io_telemMessage_init();
}

void jobs_run1Hz_tick(void)
{
    // VERY IMPORTANT that allstates after state machine
    // this is because there are fault overrides in allStates
    app_stateMachine_tick1Hz();
    app_allStates_runOnTick1Hz();
    app_stackWaterMark_check();

    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    // VERY IMPORTANT that allstates after state machine
    // this is because there are fault overrides in allStates
    app_stateMachine_tick100Hz();
    app_allStates_runOnTick100Hz();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);
}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg = io_canQueue_popRx();

    if (io_canRx_filterMessageId(rx_msg.std_id))
    {
        JsonCanMsg json_can_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_can_msg);
    }

    // check and process CAN msg for bootloader start msg
    io_bootHandler_processBootRequest(&rx_msg);

    if (app_dataCapture_needsLog((uint16_t)rx_msg.std_id, rx_msg.timestamp))
    {
        io_canLogging_loggingQueuePush(&rx_msg);
    }

    if (app_dataCapture_needsTelem((uint16_t)rx_msg.std_id, rx_msg.timestamp))
    {
        LOG_ERROR_IF(io_telemMessage_pushMsgtoQueue(&rx_msg));
    }
}
