#include "jobs.h"
#include "app_stateMachine.h"
#include "io_bootHandler.h"
#include "io_canMsg.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include <app_canTx.h>
#include <io_canRx.h>
#include <io_canTx.h>
#include "states/app_states.h"
#include "io_time.h"

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}


void jobs_init() {
    app_canTx_init();
    app_canTx_init();

    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_canQueue_init();

    app_stateMachine_init(&init_state);
}

void jobs_run1Hz_tick(void) {

    app_stateMachine_tick1Hz();
    // const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    // io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void) {
    app_stateMachine_tick100Hz();
    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void) {
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);
}

void jobs_runCanRx_tick(void) {
    const CanMsg rx_msg = io_canQueue_popRx();
    JsonCanMsg json_can_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&json_can_msg);
}


void jobs_canRxCallback(const CanMsg *rx_msg){
    if (io_canRx_filterMessageId(rx_msg->std_id)){
        io_canQueue_pushTx(rx_msg);
    }

    // check and process CAN msg for bootloader start msg
    io_bootHandler_processBootRequest(rx_msg);
}