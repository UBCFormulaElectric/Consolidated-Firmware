#include "app_heartbeatMonitor.hpp" //shared srcpp

// CAN for now
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

namespace app::heartbeatMonitors
{
static app::heartbeat::monitor<5> vc_hb_monitor(
    // bms
    io::heartbeat::node{
        app_canRx_BMS_Heartbeat_get,
        app_canRx_BMS_Heartbeat_update,
        app_canAlerts_VC_Warning_MissingBMSHeartbeat_get,
        app_canAlerts_VC_Warning_MissingBMSHeartbeat_set},
    // rsm
    io::heartbeat::node{
        app_canRx_RSM_Heartbeat_get,
        app_canRx_RSM_Heartbeat_update,
        app_canAlerts_VC_Info_MissingRSMHeartbeat_get,
        app_canAlerts_VC_Info_MissingRSMHeartbeat_set},
    // fsm
    io::heartbeat::node{
        app_canRx_FSM_Heartbeat_get,
        app_canRx_FSM_Heartbeat_update,
        app_canAlerts_VC_Warning_MissingFSMHeartbeat_get,
        app_canAlerts_VC_Warning_MissingFSMHeartbeat_set},
    // crit
    io::heartbeat::node{
        app_canRx_CRIT_Heartbeat_get,
        app_canRx_CRIT_Heartbeat_update,
        app_canAlerts_VC_Warning_MissingCRITHeartbeat_get,
        app_canAlerts_VC_Warning_MissingCRITHeartbeat_set},
    // DAM
    io::heartbeat::node{
        app_canRx_DAM_Heartbeat_get,
        app_canRx_DAM_Heartbeat_update,
        app_canAlerts_VC_Info_MissingDAMHeartbeat_get,
        app_canAlerts_VC_Info_MissingDAMHeartbeat_set} );
} // namespace app::heartbeatMonitors
