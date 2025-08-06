#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_canDataCapture.h"
#include "app_timer.h"
#include "app_canUtils.h"

#include "io_bootHandler.h"
#include "io_buzzer.h"
#include "io_log.h"
#include "io_tsim.h"

#include "io_canQueue.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "app_jsoncan.h"
#include "io_canMsg.h"
#include "io_canTx.h"
#include "io_telemMessage.h"
#include "io_telemBaseTime.h"
#include "io_time.h"
#include "io_shdn_loop.h"

#include "hw_resetReason.h"

#define RED_TOGGLE_TIME 150
#define BOOTUP_IGNORE_TIME (3000)

CanTxQueue          can_tx_queue;
static TimerChannel tsim_toggle_timer;
static TimerChannel tsim_bootup_ignore_timer;
static TsimColor    curr_tsim_color;
static TimerChannel buzzer_timeout;

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
    app_timer_init(&tsim_toggle_timer, (uint32_t)RED_TOGGLE_TIME);
    app_timer_init(&buzzer_timeout, 1000);
    curr_tsim_color = TSIM_OFF;
    app_canAlerts_DAM_Info_CanLoggingSdCardNotPresent_set(!io_fileSystem_present());

    app_timer_init(&tsim_bootup_ignore_timer, (uint32_t)BOOTUP_IGNORE_TIME);
    app_timer_restart(&tsim_bootup_ignore_timer);
    // send the telemBase time
    io_telemBaseTimeSend();
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();

    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);
    //
    io_telemBaseTimeSend();
}

void jobs_run100Hz_tick(void)
{
    io_canTx_enqueue100HzMsgs();

    const bool vc_drive_state = app_canRx_VC_State_get() == VC_DRIVE_STATE;

    TimerState timer_buzz = app_timer_runIfCondition(&buzzer_timeout, vc_drive_state);

    switch (timer_buzz)
    {
        case TIMER_STATE_RUNNING:
            io_enable_buzzer();
            break;
        default:
            io_disable_buzzer();
            break;
    }

    const bool ignore_fault_on_bootup = app_timer_updateAndGetState(&tsim_bootup_ignore_timer) == TIMER_STATE_RUNNING;

    // following TSIM outline stated in
    // https://ubcformulaelectric.atlassian.net/browse/EE-1358?isEligibleForUserSurvey=true
    const bool no_fault = (app_canRx_BMS_BmsLatchOk_get() && app_canRx_BMS_ImdLatchOk_get()) || ignore_fault_on_bootup;
    if (no_fault)
    {
        io_tsim_set_green();
        app_timer_stop(&tsim_toggle_timer);
    }
    else
    {
        static TsimColor next_tsim_color = TSIM_RED;

        switch (app_timer_updateAndGetState(&tsim_toggle_timer))
        {
            case TIMER_STATE_IDLE: // should only ever be IDLE at the very beginning when the first red command sent
                io_tsim_set_red();
                app_timer_restart(&tsim_toggle_timer);
                break;
            case TIMER_STATE_EXPIRED:
                if ((TSIM_OFF == curr_tsim_color) && (TSIM_RED == next_tsim_color))
                {
                    io_tsim_set_red();
                    app_timer_restart(&tsim_toggle_timer);
                    curr_tsim_color = TSIM_RED;
                    next_tsim_color = TSIM_OFF;
                }
                else if ((TSIM_RED == curr_tsim_color) && (TSIM_OFF == next_tsim_color))
                {
                    io_tsim_set_off();
                    app_timer_restart(&tsim_toggle_timer);
                    curr_tsim_color = TSIM_OFF;
                    next_tsim_color = TSIM_RED;
                }

                break;
            case TIMER_STATE_RUNNING:
                // do nothing, hold state until timer expires
                // LOG_PRINTF("time passed %d\n", app_timer_getElapsedTime(&tsim_toggle_timer));
                break;
            default:
                io_tsim_set_red();
                app_timer_restart(&tsim_toggle_timer);
                break;
        }
    }

    // Set shutdown node status.
    app_canTx_DAM_REStopOKStatus_set(io_r_shdn_pin_is_high());
    app_canTx_DAM_LEStopOKStatus_set(io_l_shdn_pin_is_high());
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
    io_bootHandler_processBootRequest(rx_msg);

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
