#include "jobs.h"

// app
#include "app_accumulator.h"
#include "app_stateMachine.h"
#include "app_thermistors.h"
#include "app_tractiveSystem.h"
#include "app_imd.h"
#include "app_irs.h"
#include "app_shdnLoop.h"
#include "app_heartbeatMonitors.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_segments.h"
#include "app_commitInfo.h"

// io
#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "io_canMsg.h"
#include "io_faultLatch.h"
#include "io_log.h"
#include "io_time.h"
#include "states/app_allStates.h"
#include "states/app_faultState.h"
#include "states/app_initState.h"
#include "io_bootHandler.h"
#include "io_bspdTest.h"
#include "io_canTx.h"
#include "io_canTx.h"
#include "io_canRx.h"

CanTxQueue fdcan_tx_queue;
CanTxQueue charger_tx_queue;

static void jsoncanTransmitFunc(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&fdcan_tx_queue, &msg);
}

static void chargerTransmitFunc(const JsonCanMsg *msg)
{
    // const CanMsg tx_msg = app_jsoncan_copyToCanMsg(msg);
    // assert(!tx_msg.is_fd);
    // io_canQueue_pushTx(&charger_tx_queue, &tx_msg);
    LOG_INFO("Sending message to charger");
}

void jobs_init(void)
{
    io_canTx_init(jsoncanTransmitFunc, chargerTransmitFunc);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_charger(CHARGER_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&fdcan_tx_queue);
    io_canQueue_initTx(&charger_tx_queue);

    app_tractiveSystem_init();
    app_thermistors_init();
    app_heartbeatMonitor_init(&hb_monitor);
    app_stateMachine_init(app_initState_get());
    app_allStates_init();

    app_canTx_init();
    app_canRx_init();

    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);
}

void jobs_run1Hz_tick(void)
{
    app_stateMachine_tick1Hz();
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

    app_stateMachine_tick100Hz();

    io_canTx_enqueue100HzMsgs();
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

ExitCode jobs_runLtc_tick(void)
{
    RETURN_IF_ERR(app_segments_configSync());

    // I *think* this is thread-safe?

    RETURN_IF_ERR(app_segments_broadcastCellVoltages());
    app_accumulator_calculateVoltageStats();

    RETURN_IF_ERR(app_segments_broadcastTempsVRef());
    app_accumulator_calculateTemperatureStats();

    RETURN_IF_ERR(app_segments_broadcastStatus());

    if (app_canRx_Debug_EnableDebugMode_get())
    {
        RETURN_IF_ERR(app_segments_voltageSelftest());
        RETURN_IF_ERR(app_segments_auxSelftest());
        RETURN_IF_ERR(app_segments_statusSelftest());
        RETURN_IF_ERR(app_segments_openWireCheck());
        RETURN_IF_ERR(app_segments_ADCAccuracyTest());
    }

    return EXIT_CODE_OK;
}
