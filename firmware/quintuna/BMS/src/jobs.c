#include "jobs.h"

#include "app_canRx.h"

#include "io_canTx.h"
#include "io_canRx.h"
// app
#include "app_segments.h"
// io
#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_canMsg.h"
#include "io_time.h"
#include "io_log.h"

ExitCode jobs_runLtc_tick(void)
{
    RETURN_IF_ERR(app_segments_configSync());
    RETURN_IF_ERR(app_segments_broadcastCellVoltages());
    RETURN_IF_ERR(app_segments_broadcastTempsVRef());
    RETURN_IF_ERR(app_segments_broadcastStatus());
    if (app_canRx_Debug_EnableDebugMode_get())
    {
        RETURN_IF_ERR(app_segments_openWireCheck());   // cell test
        RETURN_IF_ERR(app_segments_ADCAccuracyTest()); // cell test
    }
    return EXIT_CODE_OK;
}

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
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
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg       = io_canQueue_popRx();
    JsonCanMsg   json_can_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
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