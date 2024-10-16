#include "io_heartbeatNode.h"
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

namespace io::heartbeat
{
static node bms{ app_canRx_BMS_Heartbeat_get, [] { app_canRx_BMS_Heartbeat_update(false); },
                 app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get, app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set };
static node crit{ app_canRx_CRIT_Heartbeat_get, [] { app_canRx_CRIT_Heartbeat_update(false); },
                  app_canAlerts_VC_Fault_MissingCRITHeartbeat_get, app_canAlerts_VC_Fault_MissingCRITHeartbeat_set };
static node rsm{ app_canRx_RSM_Heartbeat_get, [] { app_canRx_RSM_Heartbeat_update(false); },
                 app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get, app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set };
static node fsm{ app_canRx_FSM_Heartbeat_get, [] { app_canRx_FSM_Heartbeat_update(false); },
                 app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get, app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_set };
} // namespace io::heartbeat
