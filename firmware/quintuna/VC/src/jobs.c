#include "jobs.h"
#include "app_stateMachine.h"
#include "io_canMsg.h"
#include "io_canQueues.h"
#include "app_jsoncan.h"
#include <app_canTx.h>
#include <io_canTx.h>
#include <stdbool.h>
#include "states/app_states.h"
#include "io_time.h"
#include "app_canRx.h"
#include "app_pumpControl.h"
#include "app_powerManager.h"

static void can1_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can1_tx_queue, &msg);
}

static void can2_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can2_tx_queue, &msg);
}

static void can3_tx(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can3_tx_queue, &msg);
}

void jobs_init()
{
    app_canTx_init();
    app_canRx_init();

    io_canTx_init(can1_tx, can2_tx, can3_tx);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_can3(CAN3_MODE_DEFAULT, true);

    io_canQueue_initRx();
    io_canQueue_initTx(&can1_tx_queue);
    io_canQueue_initTx(&can2_tx_queue);
    io_canQueue_initTx(&can3_tx_queue);

    app_stateMachine_init(&init_state);
}

void jobs_run1Hz_tick(void)
{
    app_stateMachine_tick1Hz();
    // const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    // io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    app_stateMachine_tick100Hz();
    app_powerManager_EfuseProtocolTick_100Hz();
    app_pumpControl_MonitorPumps();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);
}
