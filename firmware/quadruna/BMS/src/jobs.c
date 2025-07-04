#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_accumulator.h"
#include "app_soc.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_commitInfo.h"
#include "app_heartbeatMonitors.h"
#include "app_stackWaterMarks.h"
#include "app_tractiveSystem.h"

#include "io_canMsg.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
#include "io_bootHandler.h"

static void jsoncan_transmit(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init(void)
{
    io_canQueue_init();
    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode_Can(CAN_MODE_DEFAULT, true);

    app_canTx_init();
    app_canRx_init();
    app_accumulator_init();
    app_tractiveSystem_init();
    // Re-enable if auxiliary thermistors installed
    // app_thermistors_init();
    app_soc_init();

    app_stateMachine_init(app_initState_get());
    app_heartbeatMonitor_init(&hb_monitor);

    // broadcast commit info
    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);
}
void jobs_run1Hz_tick(void)
{
    app_stackWaterMark_check();
}
void jobs_run100Hz_tick(void) {}
void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg         = io_canQueue_popRx();
    JsonCanMsg   jsoncan_rx_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
}

void jobs_runCanRx_callBack(const CanMsg *rx_msg)
{
    if (io_canRx_filterMessageId_Can(rx_msg->std_id))
    {
        io_canQueue_pushRx(rx_msg);
    }
    // check and process CAN msg for bootloader start msg
    io_bootHandler_processBootRequest(rx_msg);
}