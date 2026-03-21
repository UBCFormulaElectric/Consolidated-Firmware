#include "app_heartbeatMonitors.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

io::heartbeat::node hb_node(
    app::can_rx::VC_Heartbeat_get,
    app::can_rx::VC_Heartbeat_update,
    app::can_tx::BMS_Info_MissingVCHeartbeat_get,
    app::can_alerts::infos::MissingVCHeartbeat_set);

std::array<io::heartbeat::node *const, 1> hb_nodes = { { &hb_node } };

const app::heartbeat::monitor<1> hb_monitor(app::can_tx::BMS_Heartbeat_set, hb_nodes);
