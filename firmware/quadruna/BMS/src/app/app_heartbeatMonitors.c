#include "app_heartbeatMonitors.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canTx.h"

static HeartbeatBoard heartbeat_boards[1] = {
    // vc
    {
        .getter       = app_canRx_VC_Heartbeat_get,
        .resetter     = app_canRx_VC_Heartbeat_update,
        .fault_setter = app_canAlerts_BMS_Warning_MissingVCHeartbeat_set,
        .fault_getter = app_canAlerts_BMS_Warning_MissingVCHeartbeat_get,
        .timeout_ms   = 200,
    }
};

HeartbeatMonitor hb_monitor = { .boards = heartbeat_boards, .board_count = 1, .block_faults = false };
