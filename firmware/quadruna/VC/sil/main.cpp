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
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "app_heartbeatMonitor.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canDataCapture.h"
#include "app_powerManager.h"
#include "app_faultCheck.h"
}

// config for heartbeat monitor (can funcs and flags)
// VC relies on FSM, RSM, BMS, CRIT
const bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = true,
    [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = true
};

// heartbeatGetters - get heartbeat signals from other boards
bool (*const heartbeatGetters[HEARTBEAT_BOARD_COUNT])(void) = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
                                                                [VC_HEARTBEAT_BOARD]   = NULL,
                                                                [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_get,
                                                                [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_get,
                                                                [DIM_HEARTBEAT_BOARD]  = NULL,
                                                                [CRIT_HEARTBEAT_BOARD] = app_canRx_CRIT_Heartbeat_get };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*const heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_update,
    [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_update,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = app_canRx_CRIT_Heartbeat_update
};

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*const heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingRSMHeartbeat_set,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = app_canAlerts_VC_Fault_MissingCRITHeartbeat_set
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*const heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingRSMHeartbeat_get,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingFSMHeartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = app_canAlerts_VC_Fault_MissingCRITHeartbeat_get
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
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_VC_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    // Start up state machine.
    app_stateMachine_init(app_initState_get());
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