#include "app_heartbeatMonitors.h"

// CAN
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

HeartbeatMonitorBoard boards[4] = {
    // bms
    { .getter       = app_canRx_BMS_Heartbeat_get,
      .resetter     = app_canRx_BMS_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Fault_MissingBMSHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Fault_MissingBMSHeartbeat_get,
      .timeout_ms   = 200 },
    // rsm
    { .getter       = app_canRx_RSM_Heartbeat_get,
      .resetter     = app_canRx_RSM_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Fault_MissingRSMHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Fault_MissingRSMHeartbeat_get,
      .timeout_ms   = 200 },
    // fsm
    { .getter       = app_canRx_FSM_Heartbeat_get,
      .resetter     = app_canRx_FSM_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Fault_MissingFSMHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Fault_MissingFSMHeartbeat_get,
      .timeout_ms   = 200 },
    // crit
    { .getter       = app_canRx_CRIT_Heartbeat_get,
      .resetter     = app_canRx_CRIT_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Fault_MissingCRITHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Fault_MissingCRITHeartbeat_get,
      .timeout_ms   = 200 }
};

HeartbeatMonitor VC_heartbeat_monitor = { .boards           = boards,
                                          .board_count      = 4,
                                          .block_faults     = false,
                                          .self_heartbeater = app_canTx_VC_Heartbeat_set };
