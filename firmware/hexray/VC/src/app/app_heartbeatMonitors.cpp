#include "app_heartbeatMonitors.hpp" //shared srcpp
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

static io::heartbeat::node bms_hb{ app::can_rx::BMS_Heartbeat_get, app::can_rx::BMS_Heartbeat_update,
                                   [](const bool b)
                                   {
                                       app::can_alerts::warnings::MissingBMSHeartbeat_set(b);
                                       app::can_rx::clear_board_rx_table(app::can_utils::CanNode::BMS_NODE);
                                   } };

static io::heartbeat::node rsm_hb{ app::can_rx::RSM_Heartbeat_get, app::can_rx::RSM_Heartbeat_update,
                                   [](const bool b)
                                   {
                                       app::can_alerts::infos::MissingRSMHeartbeat_set(b);
                                       app::can_rx::clear_board_rx_table(app::can_utils::CanNode::RSM_NODE);
                                   } };

static io::heartbeat::node fsm_hb{ app::can_rx::FSM_Heartbeat_get, app::can_rx::FSM_Heartbeat_update,
                                   [](const bool b)
                                   {
                                       app::can_alerts::warnings::MissingFSMHeartbeat_set(b);
                                       app::can_rx::clear_board_rx_table(app::can_utils::CanNode::FSM_NODE);
                                   } };

static io::heartbeat::node crit_hb{ app::can_rx::CRIT_Heartbeat_get, app::can_rx::CRIT_Heartbeat_update,
                                    [](const bool b)
                                    {
                                        app::can_alerts::warnings::MissingCRITHeartbeat_set(b);
                                        app::can_rx::clear_board_rx_table(app::can_utils::CanNode::CRIT_NODE);
                                    } };

static io::heartbeat::node dam_hb{ app::can_rx::DAM_Heartbeat_get, app::can_rx::DAM_Heartbeat_update,
                                   [](const bool b)
                                   {
                                       app::can_alerts::infos::MissingDAMHeartbeat_set(b);
                                       app::can_rx::clear_board_rx_table(app::can_utils::CanNode::DAM_NODE);
                                   } };

static constexpr std::array<std::reference_wrapper<io::heartbeat::node>, 5> heartbeat_nodes{
    { std::ref(bms_hb), std::ref(rsm_hb), std::ref(fsm_hb), std::ref(crit_hb), std::ref(dam_hb) }
};

const app::heartbeat::monitor<5> hb_monitor(app::can_tx::VC_Heartbeat_set, heartbeat_nodes);