#include "app_heartbeatMonitor.hpp" //shared srcpp
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

// Using the hearbeat nodes to monitor set up the getter and setters for the heartbeat signals.
namespace app::heartbeatMonitors
{
static io::heartbeat::node bms_hb{ app::can_rx::BMS_Heartbeat_get, app::can_rx::BMS_Heartbeat_update,
                                   app::can_tx::VC_Warning_MissingBMSHeartbeat_get,
                                   app::can_alerts::warnings::MissingBMSHeartbeat_set };

static io::heartbeat::node rsm_hb{ app::can_rx::RSM_Heartbeat_get, app::can_rx::RSM_Heartbeat_update,
                                   app::can_tx::VC_Info_MissingRSMHeartbeat_get,
                                   app::can_alerts::infos::MissingRSMHeartbeat_set };

static io::heartbeat::node fsm_hb{ app::can_rx::FSM_Heartbeat_get, app::can_rx::FSM_Heartbeat_update,
                                   app::can_tx::VC_Warning_MissingFSMHeartbeat_get,
                                   app::can_alerts::warnings::MissingFSMHeartbeat_set };

static io::heartbeat::node crit_hb{ app::can_rx::CRIT_Heartbeat_get, app::can_rx::CRIT_Heartbeat_update,
                                    app::can_tx::VC_Warning_MissingCRITHeartbeat_get,
                                    app::can_alerts::warnings::MissingCRITHeartbeat_set };

static io::heartbeat::node dam_hb{ app::can_rx::DAM_Heartbeat_get, app::can_rx::DAM_Heartbeat_update,
                                   app::can_tx::VC_Info_MissingDAMHeartbeat_get,
                                   app::can_alerts::infos::MissingDAMHeartbeat_set };

static app::heartbeat::monitor<5> vc_hbs(
    app::can_tx::VC_Heartbeat_set,
    std::array<io::heartbeat::node *const, 5>{ { &bms_hb, &rsm_hb, &fsm_hb, &crit_hb, &dam_hb } });
} // namespace app::heartbeatMonitors
