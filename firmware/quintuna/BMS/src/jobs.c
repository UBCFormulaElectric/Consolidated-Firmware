#include "jobs.h"

#include "app_canTx.h"
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
#include "io_ltc6813.h"
#include "io_time.h"

void jobs_runLtc_tick(void)
{
    app_segments_configSync();
    app_segments_broadcastVoltages();
    app_segments_broadcastTempsVRef();
    app_segments_broadcastStatus();
    if (app_canRx_Debug_EnableDebugMode_get())
    {
        app_segments_openWireCheck();   // cell test
        app_segments_ADCAccuracyTest(); // cell test
    }
}

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init()
{
    io_canQueue_init();
    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
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
    if (io_canRx_filterMessageId(rx_msg->std_id))
    {
        io_canQueue_pushRx(rx_msg);
    }

    io_bootHandler_processBootRequest(rx_msg);
}