#include "app_heartbeatMonitors.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"
#include <functional>

static io::heartbeat::node vc_heartbeat_node(
    app::can_rx::VC_Heartbeat_get,
    app::can_rx::VC_Heartbeat_update,
    [](const bool fault)
    {
        if (fault)
            app::can_rx::clear_board_rx_table(app::can_utils::CanNode::VC_NODE);
        app::can_alerts::infos::MissingVCHeartbeat_set(fault);
    });
static io::heartbeat::node crit_heartbeat_node(
    app::can_rx::CRIT_Heartbeat_get,
    app::can_rx::CRIT_Heartbeat_update,
    [](const bool fault)
    {
        if (fault)
            app::can_rx::clear_board_rx_table(app::can_utils::CanNode::CRIT_NODE);
    });
static io::heartbeat::node dam_heartbeat_node(
    app::can_rx::DAM_Heartbeat_get,
    app::can_rx::DAM_Heartbeat_update,
    [](const bool fault)
    {
        if (fault)
            app::can_rx::clear_board_rx_table(app::can_utils::CanNode::DAM_NODE);
    });
static io::heartbeat::node fsm_heartbeat_node(
    app::can_rx::FSM_Heartbeat_get,
    app::can_rx::FSM_Heartbeat_update,
    [](const bool fault)
    {
        if (fault)
            app::can_rx::clear_board_rx_table(app::can_utils::CanNode::FSM_NODE);
    });
static io::heartbeat::node rsm_heartbeat_node(
    app::can_rx::RSM_Heartbeat_get,
    app::can_rx::RSM_Heartbeat_update,
    [](const bool fault)
    {
        if (fault)
            app::can_rx::clear_board_rx_table(app::can_utils::CanNode::RSM_NODE);
    });

static std::array<std::reference_wrapper<io::heartbeat::node>, 5> hb_nodes{
    { vc_heartbeat_node, crit_heartbeat_node, dam_heartbeat_node, fsm_heartbeat_node, rsm_heartbeat_node }
};
const app::heartbeat::monitor<5> hb_monitor(app::can_tx::BMS_Heartbeat_set, hb_nodes);
