#include "app_heartbeatMonitor.hpp" //shared srcpp
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"

// Using the hearbeat nodes to monitor set up the getter and setters for the heartbeat signals.
namespace app::heartbeatMonitors
{
static app::heartbeat::monitor<5> vc_hbs(
    // bms
    io::heartbeat::node{ app::can_rx::BMS_Heartbeat_get, app::can_rx::BMS_Heartbeat_update,
                         app::can_alerts::VC_Warning_MissingBMSHeartbeat_get,
                         app::can_alerts::VC_Warning_MissingBMSHeartbeat_set },
    // rsm
    io::heartbeat::node{ app::can_rx::RSM_Heartbeat_get, app::can_rx::RSM_Heartbeat_update,
                         app::can_alerts::VC_Info_MissingRSMHeartbeat_get,
                         app::can_alerts::VC_Info_MissingRSMHeartbeat_set },
    // fsm
    io::heartbeat::node{ app::can_rx::FSM_Heartbeat_get, app::can_rx::FSM_Heartbeat_update,
                         app::can_alerts::VC_Warning_MissingFSMHeartbeat_get,
                         app::can_alerts::VC_Warning_MissingFSMHeartbeat_set },
    // crit
    io::heartbeat::node{ app::can_rx::CRIT_Heartbeat_get, app::can_rx::CRIT_Heartbeat_update,
                         app::can_alerts::VC_Warning_MissingCRITHeartbeat_get,
                         app::can_alerts::VC_Warning_MissingCRITHeartbeat_set },
    // DAM
    io::heartbeat::node{ app::can_rx::DAM_Heartbeat_get, app::can_rx::DAM_Heartbeat_update,
                         app::can_alerts::VC_Info_MissingDAMHeartbeat_get,
                         app::can_alerts::VC_Info_MissingDAMHeartbeat_set });
} // namespace app::heartbeatMonitors
