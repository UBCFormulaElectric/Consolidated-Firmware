#include "io_heartbeatNode.h"
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

namespace io::heartbeat
{
static const node bms{ app_canRx_BMS_Heartbeat_get, [] { app_canRx_BMS_Heartbeat_update(false); },
                       app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get,
                       app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set };
static const node vc{ app_canRx_RVC_Heartbeat_get, [] { app_canRx_RVC_Heartbeat_update(false); },
                      app_canAlerts_CRIT_Fault_MissingRVCHeartbeat_get,
                      app_canAlerts_CRIT_Fault_MissingRVCHeartbeat_set };
static const node rsm{ app_canRx_RSM_Heartbeat_get, [] { app_canRx_RSM_Heartbeat_update(false); },
                       app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get,
                       app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set };
static const node fsm{ app_canRx_FVC_Heartbeat_get, [] { app_canRx_FVC_Heartbeat_update(false); },
                       app_canAlerts_CRIT_Fault_MissingFVCHeartbeat_get,
                       app_canAlerts_CRIT_Fault_MissingFVCHeartbeat_set };
} // namespace io::heartbeat