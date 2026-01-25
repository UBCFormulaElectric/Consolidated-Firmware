#include "app_heartbeatMonitors.hpp"
extern "C"
{
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
}

io::heartbeat::node hb_node(
    app_canRx_FSM_Heartbeat_get,
    app_canRx_FSM_Heartbeat_update,
    app_canAlerts_RSM_Info_MissingFSMHeartbeat_get,
    app_canAlerts_RSM_Info_MissingFSMHeartbeat_set);


std::array<io::heartbeat::node *const, 1> hb_nodes = {{ &hb_node }};

const app::heartbeat::monitor<1> hb_monitor(app_canTx_RSM_Heartbeat_set, hb_nodes);

