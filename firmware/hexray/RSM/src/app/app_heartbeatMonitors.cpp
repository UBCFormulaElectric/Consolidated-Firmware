#include "app_heartbeatMonitors.hpp"
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

static HeartbeatBoard heartbeat_boards[1] = { { false,
                                                false,
                                                { 0, TIMER_STATE_IDLE, 0 },
                                                0,
                                                app_canRx_FSM_Heartbeat_get,
                                                app_canRx_FSM_Heartbeat_update,
                                                app_canAlerts_RSM_Info_MissingFSMHeartbeat_set,
                                                app_canAlerts_RSM_Info_MissingFSMHeartbeat_get } };

const HeartbeatMonitor hb_monitor = { .boards = heartbeat_boards, .board_count = 1, .block_faults = false };