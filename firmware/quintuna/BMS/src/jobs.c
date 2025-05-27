#include "jobs.h"

#include "app_shdnLoop.h"
#include "app_heartbeatMonitors.h"
#include "app_canTx.h"

#include "io_bootHandler.h"
#include "io_canTx.h"
#include "io_faultLatch.h"

#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "io_canMsg.h"
#include "io_canRx.h"
#include "io_log.h"

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

static void charger_transmit_func(const JsonCanMsg *msg)
{
    LOG_INFO("Send charger message: %d", msg->std_id);
}

void jobs_init()
{
    io_canTx_init(jsoncan_transmit_func, charger_transmit_func);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_charger(CHARGER_MODE_DEFAULT, true);
    io_canQueue_init();

    app_heartbeatMonitor_init(&hb_monitor);
    app_canTx_BMS_Heartbeat_set(true);
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();

    /**
     * add charger connection status CAN tx once charging implementation branch is merged
     */

     /**
      * add reset SOC from min cell voltage if soc corrupt and voltage readings settled after soc app is finished
      */
}

void jobs_run100Hz_tick(void)
{
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
    /**
     * app_canTx_BMS_Soc_set(app_soc_getMinSocPercent()); add once soc app is finished
     */

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

void jobs_run1kHz_tick(void) {}

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