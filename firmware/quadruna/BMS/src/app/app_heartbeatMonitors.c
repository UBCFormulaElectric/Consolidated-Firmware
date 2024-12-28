//
// Created by jessi on 2024-12-28.
//

#include "app_heartbeatMonitors.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canTx.h"

static HeartbeatMonitorBoard boards[1] = {
    // vc
    {
        .getter       = app_canRx_VC_Heartbeat_get,
        .resetter     = app_canRx_VC_Heartbeat_update,
        .fault_setter = app_canAlerts_BMS_Warning_MissingVCHeartbeat_set,
        .fault_getter = app_canAlerts_BMS_Warning_MissingVCHeartbeat_get,
        .timeout_ms   = 200,
    }
};

const HeartbeatMonitor BMS_heartbeat_monitor = { .boards           = boards,
                                                 .board_count      = 1,
                                                 .block_faults     = false,
                                                 .self_heartbeater = app_canTx_BMS_Heartbeat_set };
