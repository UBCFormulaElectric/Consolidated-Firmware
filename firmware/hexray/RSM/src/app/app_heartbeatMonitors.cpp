#include "app_heartbeatMonitors.hpp"
extern "C"
{
    #include "app_canTx.h"
    #include "app_canAlerts.h" //Not created yet
}

// int app_canRx_FSM_Heartbeat_get = 0;
// int app_canRx_FSM_Heartbeat_update = 0;
// int app_canAlerts_RSM_Info_MissingFSMHeartbeat_set = 0;
// int app_canAlerts_RSM_Info_MissingFSMHeartbeat_get = 0;

// .getter etc not working & struct not working
static HeartbeatBoard heartbeat_boards[1] = {
    {
        .getter       = app_canRx_FSM_Heartbeat_get(),
        .resetter     = app_canRx_FSM_Heartbeat_update(),
        .fault_setter = app_canAlerts_RSM_Info_MissingFSMHeartbeat_set(),
        .fault_getter = app_canAlerts_RSM_Info_MissingFSMHeartbeat_get(),
        .timeout_ms   = 3000,
    }
};

const HeartbeatMonitor hb_monitor = {.boards = heartbeat_boards, .board_count = 1, .block_faults = false};