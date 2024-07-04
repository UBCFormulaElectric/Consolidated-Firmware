#include "io_heartbeatNode.h"
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

namespace io::heartbeat
{
static const node bms{ app_canRx_BMS_Heartbeat_get, [] { app_canRx_BMS_Heartbeat_update(false); },
                       app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set,
                       app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get };
static const node vc{ app_canRx_VC_Heartbeat_get, [] { app_canRx_VC_Heartbeat_update(false); },
                      app_canAlerts_CRIT_Fault_MissingVCHeartbeat_set,
                      app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get };
static const node rsm{ app_canRx_RSM_Heartbeat_get, [] { app_canRx_RSM_Heartbeat_update(false); },
                       app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set,
                       app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get };
static const node fsm{ app_canRx_FSM_Heartbeat_get, [] { app_canRx_FSM_Heartbeat_update(false); },
                       app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_set,
                       app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get };
} // namespace io::heartbeat