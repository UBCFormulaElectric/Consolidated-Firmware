#include "app_heartbeatMonitors.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include <functional>

static io::heartbeat::node vc_hb_node(
    app::can_rx::VC_Heartbeat_get,
    app::can_rx::VC_Heartbeat_update,
    app::can_tx::BMS_Info_MissingVCHeartbeat_get,
    [](const bool fault)
    {
        if (fault)
            app::can_rx::clear_board_rx_table(app::can_utils::CanNode::VC_NODE);
        app::can_alerts::infos::MissingVCHeartbeat_set(fault);
    });

static std::array<std::reference_wrapper<io::heartbeat::node>, 1> hb_nodes{ { vc_hb_node } };
const app::heartbeat::monitor<1>                                  hb_monitor(app::can_tx::BMS_Heartbeat_set, hb_nodes);
