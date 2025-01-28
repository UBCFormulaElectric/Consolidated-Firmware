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

#include "io_canMsg.h"
#include "io_canQueue.h"
#include "io_cans.h"
#include "io_jsoncan.h"

static void jsoncan_transmit(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init(void)
{
    io_can_init(&can1);
    io_canQueue_init();
    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);

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
}
void jobs_run1Hz_tick(void)
{
    app_stackWaterMark_check();
}
void jobs_run100Hz_tick(void) {}
void jobs_run1kHz_tick(void) {}

void jobs_runCanTx_tick(void)
{
    CanMsg tx_msg = io_canQueue_popTx();
    io_can_transmit(&can1, &tx_msg);
}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg         = io_canQueue_popRx();
    JsonCanMsg   jsoncan_rx_msg = io_jsoncan_copyFromCanMsg(&rx_msg);

    io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
}