#include "app_heartbeatMonitor.hpp" //shared srcpp
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

static io::heartbeat::node bms_hb{ app::can_rx::BMS_Heartbeat_get, app::can_rx::BMS_Heartbeat_update,
                                   app::can_alerts::warnings::MissingBMSHeartbeat_set };

static io::heartbeat::node rsm_hb{ app::can_rx::RSM_Heartbeat_get, app::can_rx::RSM_Heartbeat_update,
                                   app::can_alerts::infos::MissingRSMHeartbeat_set };

static io::heartbeat::node fsm_hb{ app::can_rx::FSM_Heartbeat_get, app::can_rx::FSM_Heartbeat_update,
                                   app::can_alerts::warnings::MissingFSMHeartbeat_set };

static io::heartbeat::node crit_hb{ app::can_rx::CRIT_Heartbeat_get, app::can_rx::CRIT_Heartbeat_update,
                                    app::can_alerts::warnings::MissingCRITHeartbeat_set };

static io::heartbeat::node dam_hb{ app::can_rx::DAM_Heartbeat_get, app::can_rx::DAM_Heartbeat_update,
                                   app::can_alerts::infos::MissingDAMHeartbeat_set };

app::heartbeat::monitor hb_monitor(
    app::can_tx::VC_Heartbeat_set,
    std::array<std::reference_wrapper<io::heartbeat::node>, 5>{ { bms_hb, rsm_hb, fsm_hb, crit_hb, dam_hb } });