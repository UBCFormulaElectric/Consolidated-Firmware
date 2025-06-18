#include "jobs.h"
#include "app_stateMachine.h"
#include "app_timer.h"
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
#include "app_powerMonitoring.h"
#include "app_commitInfo.h"
#include "app_canRx.h"
#include "app_warningHanding.h"

#define AIR_MINUS_OPEN_DEBOUNCE_MS (1000U)

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

static TimerChannel air_minus_open_debounce_timer;

void jobs_init()
{
    app_canTx_init();
    app_canRx_init();

    io_canTx_init(can1_tx, can2_tx, can3_tx);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_can2(CAN2_MODE_DEFAULT, true);
    io_canTx_enableMode_can3(CAN3_MODE_DEFAULT, true);

    io_canQueue_initRx();
    io_canQueue_initTx(&can1_tx_queue);
    io_canQueue_initTx(&can2_tx_queue);
    io_canQueue_initTx(&can3_tx_queue);

    app_stateMachine_init(&init_state);

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_VC_Heartbeat_set(true);

    app_timer_init(&air_minus_open_debounce_timer, AIR_MINUS_OPEN_DEBOUNCE_MS);

    app_softwareBspd_init();
}

void jobs_run1Hz_tick(void)
{
    // const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    // io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);

    app_stateMachine_tickTransitionState();
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    const bool air_minus_open_debounced =
        app_timer_runIfCondition(&air_minus_open_debounce_timer, !app_canRx_BMS_IrNegative_get());
    if (air_minus_open_debounced)
    {
        app_stateMachine_setNextState(&init_state);
    }
    else
    {
        app_stateMachine_tick100Hz();
    }

    app_powerManager_EfuseProtocolTick_100Hz();
    app_pumpControl_MonitorPumps();

    app_stateMachine_tickTransitionState();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);
}

void jobs_runPowerMonitoring_tick(void)
{
    app_powerMonitoring_update();
}