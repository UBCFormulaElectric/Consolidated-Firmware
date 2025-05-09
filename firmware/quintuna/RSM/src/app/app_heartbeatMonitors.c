#include "app_heartbeatMonitors.h"

// #include "app_canTx.h"
// #include "app_canRx.h"
// #include "app_canAlerts.h"

// static HeartbeatBoard heartbeat_boards[3] = {
//     // bms
//     { .getter       = app_canRx_BMS_Heartbeat_get,
//       .resetter     = app_canRx_BMS_Heartbeat_update,
//       .fault_setter = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_set,
//       .fault_getter = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_get,
//       .timeout_ms   = 200 },
//     // vc
//     { .getter       = app_canRx_VC_Heartbeat_get,
//       .resetter     = app_canRx_VC_Heartbeat_update,
//       .fault_setter = app_canAlerts_RSM_Fault_MissingVCHeartbeat_set,
//       .fault_getter = app_canAlerts_RSM_Fault_MissingVCHeartbeat_get,
//       .timeout_ms   = 200 },
//     // fsm
//     { .getter       = app_canRx_FSM_Heartbeat_get,
//       .resetter     = app_canRx_FSM_Heartbeat_update,
//       .fault_setter = app_canAlerts_RSM_Fault_MissingFSMHeartbeat_set,
//       .fault_getter = app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get,
//       .timeout_ms   = 200 }
// };

// const HeartbeatMonitor hb_monitor = { .boards = heartbeat_boards, .board_count = 3, .block_faults = false };