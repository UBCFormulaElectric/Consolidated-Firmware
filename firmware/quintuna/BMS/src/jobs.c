#include "jobs.h"

#include "app_globals.h"
#include "app_soc.h"
#include "app_shdnLoop.h"
#include "app_heartbeatMonitors.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_segments.h"

#include "io_bootHandler.h"
#include "io_canTx.h"
#include "io_faultLatch.h"
#include "io_sds.h"

#include "io_canTx.h"
#include "io_canRx.h"
// app
#include "app_commitInfo.h"
// io
#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "io_canMsg.h"
#include "io_time.h"
#include "io_log.h"
#include "io_irs.h"

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

    // TODO: Uncomment after soc is tested
    // app_soc_init();
    // app_init_globals()
    app_heartbeatMonitor_init(&hb_monitor);

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();

    // TODO: Unomment after soc is tested
    // const float min_soc = app_soc_getMinSocCoulombs();
    // if (min_soc < 0)
    // {
    //     if (globals->cell_monitor_settle_count >= NUM_CYCLES_TO_SETTLE)
    //     {
    //         app_soc_resetSocFromVoltage();
    //     }
    // }
    // else
    // {
    //     // Send SD card write request via queue
    //     SdRequest req = {
    //         .type        = SD_REQ_WRITE_SOC,
    //         .soc_value   = min_soc,
    //         .done_sem    = NULL,
    //         .success_ptr = NULL,
    //         .result_ptr  = NULL,
    //     };
    //     io_sds_enqueue(&req);
    // }
}

void jobs_run100Hz_tick(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    io_canTx_enqueue100HzMsgs();
    app_shdnLoop_broadcast();

    app_canTx_BMS_Heartbeat_set(true);

    /**
     * Update CAN signals for BMS latch statuses
     */
    app_canTx_BMS_BmsOk_set(io_faultLatch_getCurrentStatus(&bms_ok_latch));
    app_canTx_BMS_ImdOk_set(io_faultLatch_getCurrentStatus(&imd_ok_latch));
    app_canTx_BMS_BspdOk_set(io_faultLatch_getCurrentStatus(&bspd_ok_latch));
    app_canTx_BMS_BmsLatchedFault_set(io_faultLatch_getLatchedStatus(&bms_ok_latch));
    app_canTx_BMS_ImdLatchedFault_set(io_faultLatch_getLatchedStatus(&imd_ok_latch));
    app_canTx_BMS_BspdLatchedFault_set(io_faultLatch_getLatchedStatus(&bspd_ok_latch));

    // TODO: Uncomment after soc is tested
    // app_canTx_BMS_Soc_set(app_soc_getMinSocPercent());
    // if (io_irs_isNegativeClosed() && io_irs_isPositiveClosed())
    // {
    //     app_soc_updateSocStats();
    // }
    // if (globals->cell_monitor_settle_count < NUM_CYCLES_TO_SETTLE)
    // {
    //     globals->cell_monitor_settle_count++;
    // }
    /**
     * add cell balancing check once cell balancing is enabled
     */

    /**
     * add accumulator tasks once accumulator app is finished
     */

    /**
     * add soc stat update once app soc is finished
     */
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

void jobs_canRxCallback(const CanMsg *rx_msg)
{
    if (io_canRx_filterMessageId_can1(rx_msg->std_id))
    {
        io_canQueue_pushRx(rx_msg);
    }

    io_bootHandler_processBootRequest(rx_msg);
}

void jobs_runSdCard_tick(void)
{
    SdRequest          req;
    osMessageQueueId_t queue = io_sds_queue_handle();
    if (queue && osMessageQueueGet(queue, &req, NULL, 0) == osOK)
    {
        bool success = false;
        switch (req.type)
        {
            case SD_REQ_WRITE_SOC:
                success = app_soc_writeSocToSd(req.soc_value);
                break;
            case SD_REQ_READ_SOC:
                if (req.result_ptr)
                    success = app_soc_readSocFromSd(req.result_ptr);
                break;
            default:
                break;
        }
        if (req.success_ptr)
            *(req.success_ptr) = success;
        if (req.done_sem)
            osSemaphoreRelease(req.done_sem);
    }
}