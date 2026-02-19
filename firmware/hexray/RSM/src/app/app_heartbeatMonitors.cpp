#include "app_heartbeatMonitors.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

io::heartbeat::node hb_node(
    app::can_rx::FSM_Heartbeat_get,
    app::can_rx::FSM_Heartbeat_update,
    app::can_tx::RSM_Info_MissingFSMHeartbeat_get,
    app::can_alerts::infos::MissingFSMHeartbeat_set);

std::array<io::heartbeat::node *const, 1> hb_nodes = { { &hb_node } };

const app::heartbeat::monitor<1> hb_monitor(app::can_tx::RSM_Heartbeat_set, hb_nodes);
