#include "app_heartbeatMonitors.h"

#include "app_canRx.h"
#include "app_canAlerts.h"

static HeartbeatBoard heartbeat_boards[5] = {
    // bms
    { .getter       = app_canRx_BMS_Heartbeat_get,
      .resetter     = app_canRx_BMS_Heartbeat_update,
      .fault_setter = app_canAlerts_CRIT_Info_MissingBMSHeartbeat_set,
      .fault_getter = app_canAlerts_CRIT_Info_MissingBMSHeartbeat_get,
      .timeout_ms   = 3000 },
    // vc
    { .getter       = app_canRx_VC_Heartbeat_get,
      .resetter     = app_canRx_VC_Heartbeat_update,
      .fault_setter = app_canAlerts_CRIT_Info_MissingVCHeartbeat_set,
      .fault_getter = app_canAlerts_CRIT_Info_MissingVCHeartbeat_get,
      .timeout_ms   = 3000 },
    // rsm
    { .getter       = app_canRx_RSM_Heartbeat_get,
      .resetter     = app_canRx_RSM_Heartbeat_update,
      .fault_setter = app_canAlerts_CRIT_Info_MissingRSMHeartbeat_set,
      .fault_getter = app_canAlerts_CRIT_Info_MissingRSMHeartbeat_get,
      .timeout_ms   = 3000 },
    // fsm
    { .getter       = app_canRx_FSM_Heartbeat_get,
      .resetter     = app_canRx_FSM_Heartbeat_update,
      .fault_setter = app_canAlerts_CRIT_Info_MissingFSMHeartbeat_set,
      .fault_getter = app_canAlerts_CRIT_Info_MissingFSMHeartbeat_get,
      .timeout_ms   = 3000 },
    // dam
    { .getter       = app_canRx_DAM_Heartbeat_get,
      .resetter     = app_canRx_DAM_Heartbeat_update,
      .fault_setter = app_canAlerts_CRIT_Info_MissingDAMHeartbeat_set,
      .fault_getter = app_canAlerts_CRIT_Info_MissingDAMHeartbeat_get,
      .timeout_ms   = 3000 }
};

HeartbeatMonitor hb_monitor = { .boards = heartbeat_boards, .board_count = 5, .block_faults = false };
