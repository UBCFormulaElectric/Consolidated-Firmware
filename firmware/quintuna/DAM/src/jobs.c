#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_stackWaterMarks.h"
#include "app_canDataCapture.h"

#include "io_buzzer.h"
#include "io_log.h"
#include "io_tsim.h"

#include "io_canQueue.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "app_jsoncan.h"
#include "io_canMsg.h"
#include "io_time.h"
#include "io_telemMessage.h"
#include "io_telemBaseTime.h"

#include "hw_resetReason.h"

CanTxQueue can_tx_queue;

static void can1_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
    if (io_canLogging_errorsRemaining() > 0 && app_dataCapture_needsLog((uint16_t)msg.std_id, msg.timestamp))
    {
        io_canLogging_loggingQueuePush(&msg);
    }

    if (app_dataCapture_needsTelem((uint16_t)msg.std_id, msg.timestamp))
    {
        // Should make this log an error but it spams currently...
        // Consider doing a "num errors remaining" strategy like CAN logging.
        io_telemMessage_pushMsgtoQueue(&msg);
    }
}

void jobs_init()
{
    io_canTx_init(can1_tx);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);
    io_telemMessage_init();
    io_rtc_init();
    io_tsim_set_off();
    io_telemBaseTimeInit();

    app_canTx_DAM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_DAM_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_DAM_Heartbeat_set(true);
    app_canTx_DAM_ResetReason_set((CanResetReason)hw_resetReason_get());

    app_canAlerts_DAM_Info_CanLoggingSdCardNotPresent_set(!io_fileSystem_present());
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();

    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);
    //
    io_telemBaseTimeSend();
    app_stackWaterMark_check();
}

void jobs_run100Hz_tick(void)
{
    io_canTx_enqueue100HzMsgs();

    const bool buzzer_control = app_canRx_VC_BuzzerControl_get();
    if (buzzer_control)
    {
        io_enable_buzzer();
    }
    else
    {
        io_disable_buzzer();
    }

    const TsimColor tsimColor = app_canRx_VC_TsimControl_get();
    if (tsimColor == TSIM_GREEN)
    {
        io_tsim_set_red(true);
    }
    else if (tsimColor == TSIM_RED)
    {
        io_tsim_set_red(true);
    }
    else if (tsimColor == TSIM_OFF)
    {
        io_tsim_set_off();
    }
}

void jobs_run1kHz_tick(void)
{
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);
}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg       = io_canQueue_popRx();
    JsonCanMsg   json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&json_can_msg);
}

void jobs_runCanRx_callBack(const CanMsg *rx_msg)
{
    if (io_canRx_filterMessageId_can1(rx_msg->std_id))
    {
        io_canQueue_pushRx(rx_msg);
    }
    if (io_canLogging_errorsRemaining() > 0 && app_dataCapture_needsLog((uint16_t)rx_msg->std_id, rx_msg->timestamp))
    {
        io_canLogging_loggingQueuePush(rx_msg);
    }

    if (app_dataCapture_needsTelem((uint16_t)rx_msg->std_id, rx_msg->timestamp))
    {
        // Should make this log an error but it spams currently...
        // Consider doing a "num errors remaining" strategy like CAN logging.
        io_telemMessage_pushMsgtoQueue(rx_msg);
    }
    // check and process CAN msg for bootloader start msg
}
