#include "app_canRx.hpp"
#include "app_switches.hpp"
#include "app_leds.hpp"

#include "app_brightness.hpp"
#include "io_leds.hpp"
#include "app_canAlerts.hpp"
#include "app_heartbeatMonitors.hpp"

namespace app::leds
{
static io::leds::color board_status(const bool fault, const bool warning, const bool heartbeat)
{
    if (not heartbeat)
        return io::leds::color::OFF;
    if (warning)
        return io::leds::color::YELLOW;
    if (fault)
        return io::leds::color::RED;
    return io::leds::color::GREEN;
}

void init()
{
    // TODO move this to app_leds
    result<void> ec = io::leds::update(io::leds::config{
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        io::leds::color::OFF,
        false,
        false,
        false,
        false,
        false,
    });

    LOG_IF_ERR(ec);

    ec = io::leds::setBrightness(1);

    LOG_IF_ERR(ec);
}

void setLeds()
{
    /**
     * TODO:
     *
     * push to drive:
     * once in hv state (or whatever is right before drive state),
     * blinky drive led to let driver know they can go into drive,
     * then set green on successful state transition
     *
     * launch control:
     * set led to yellow when enabled, purple when ready, and then
     * off when pressed off to actually launch the vehicle
     * ready condition based on pedal percentage
     */
    LOG_IF_ERR(io::leds::update(io::leds::config{
        board_status(
            can_alerts::BoardHasFault(can_utils::CanNode::RSM_NODE),
            can_alerts::BoardHasWarning(can_utils::CanNode::RSM_NODE), rsm_heartbeat_node.status),
        board_status(
            can_alerts::BoardHasFault(can_utils::CanNode::BMS_NODE),
            can_alerts::BoardHasWarning(can_utils::CanNode::BMS_NODE), bms_heartbeat_node.status),
        board_status(
            can_alerts::BoardHasFault(can_utils::CanNode::VC_NODE),
            can_alerts::BoardHasWarning(can_utils::CanNode::VC_NODE), vc_heartbeat_node.status),
        board_status(
            can_alerts::BoardHasFault(can_utils::CanNode::FSM_NODE),
            can_alerts::BoardHasWarning(can_utils::CanNode::FSM_NODE), fsm_heartbeat_node.status),
        io::leds::color::GREEN,
        board_status(
            can_alerts::BoardHasFault(can_utils::CanNode::DAM_NODE),
            can_alerts::BoardHasWarning(can_utils::CanNode::DAM_NODE), dam_heartbeat_node.status),
        switches::launch_control_get() ? io::leds::color::GREEN : io::leds::color::OFF,
        app::can_rx::VC_State_get() == can_utils::VCState::VC_DRIVE_STATE ? io::leds::color::GREEN
                                                                          : io::leds::color::OFF,
        can_rx::VC_FirstFaultNode_get() == can_utils::ShutdownNode::OK ? io::leds::color::OFF : io::leds::color::RED,
        switches::regen_get(),
        switches::torque_vectoring_get(),
        !can_rx::BMS_ImdCurrentlyOk_get(),
        !can_rx::BMS_BmsCurrentlyOk_get(),
        !can_rx::BMS_BspdCurrentlyOk_get(),
    }));
    LOG_IF_ERR(io::leds::setBrightness(app::brightness));
}
} // namespace app::leds
