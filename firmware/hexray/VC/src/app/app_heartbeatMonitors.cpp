#include "app_heartbeatMonitors.h"

// CAN
extern "C" {
#include "app_canRx.h"
#include "app_canAlerts.h"
}

namespace vc::app::heartbeat_monitors {
static HeartbeatBoard heartbeat_boards[6] = {
    // bms
    { false, false, {}, 200,
        app_canRx_BMS_Heartbeat_get,
       app_canRx_BMS_Heartbeat_update,
      app_canAlerts_VC_Warning_MissingBMSHeartbeat_set,
      app_canAlerts_VC_Warning_MissingBMSHeartbeat_get,
      },
    // rsm
    {     { false, false, {}, 200,
 app_canRx_RSM_Heartbeat_get,
  app_canRx_RSM_Heartbeat_update,
 app_canAlerts_VC_Info_MissingRSMHeartbeat_set,
app_canAlerts_VC_Info_MissingRSMHeartbeat_get,
},
    // fsm
    { .getter       = app_canRx_FSM_Heartbeat_get,
      .resetter     = app_canRx_FSM_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Warning_MissingFSMHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Warning_MissingFSMHeartbeat_get,
      .timeout_ms   = 200 },
    // crit
    { .getter       = app_canRx_CRIT_Heartbeat_get,
      .resetter     = app_canRx_CRIT_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Warning_MissingCRITHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Warning_MissingCRITHeartbeat_get,
      .timeout_ms   = 200 },
    // vcr
    { .getter       = app_canRx_VCR_Heartbeat_get,
      .resetter     = app_canRx_VCR_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Info_MissingVCRHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Info_MissingVCRHeartbeat_get,
      .timeout_ms   = 200 },
    // DAM
    { .getter       = app_canRx_DAM_Heartbeat_get,
      .resetter     = app_canRx_DAM_Heartbeat_update,
      .fault_setter = app_canAlerts_VC_Info_MissingDAMHeartbeat_set,
      .fault_getter = app_canAlerts_VC_Info_MissingDAMHeartbeat_get,
      .timeout_ms   = 200 }
};

const HeartbeatMonitor hb_monitor = { .boards = heartbeat_boards, .board_count = 6, .block_faults = false };
?
}
} // namespace vc::app::heartbeat_monitors