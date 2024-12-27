#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CZMQ for sockets.
#include <czmq.h>

// SIL board functions.
extern "C"
{
#include "sil.h"
}

// IO layer.
extern "C"
{
#include "io_canRx.h"
#include "io_canTx.h"
}

// App layer.
extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_heartbeatMonitor.h"
}

// Config for heartbeat monitor.
// CRIT rellies on BMS, VC, RSM, FSM.
static const bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = true,   [RSM_HEARTBEAT_BOARD] = true,
    [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
};

// heartbeatGetters - get heartbeat signals from other boards.
static bool (*const heartbeatGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
    [VC_HEARTBEAT_BOARD]   = app_canRx_VC_Heartbeat_get,
    [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_get,
    [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatUpdaters - update local CAN table with heartbeat status.
static void (*const heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
    [VC_HEARTBEAT_BOARD]   = app_canRx_VC_Heartbeat_update,
    [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_update,
    [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_update,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultSetters - broadcast heartbeat faults over CAN.
static void (*const heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_set,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN.
static bool (*const heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

void tasks_init()
{
    // Setup can with fake callback for transmission.
    io_canTx_init(sil_txCallback);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();

    // Setup heartbeat monitor.
    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_CRIT_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    // Start up state machine.
    app_stateMachine_init(app_mainState_get());
}

void tasks_1kHz(uint32_t time_ms)
{
    io_canTx_enqueueOtherPeriodicMsgs(time_ms);
}

void tasks_100Hz(uint32_t time_ms)
{
    app_stateMachine_tick100Hz();
    io_canTx_enqueue100HzMsgs();
}

void tasks_1Hz(uint32_t time_ms)
{
    app_stateMachine_tick1Hz();
    io_canTx_enqueue1HzMsgs();
}

int main(int argc, char *argv[])
{
    sil_main(argc, argv, tasks_init, tasks_1Hz, tasks_100Hz, tasks_1kHz);
}