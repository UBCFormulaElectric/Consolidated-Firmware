#include "app_canRx.hpp"
#include "app_switches.hpp"
#include "app_leds.hpp"
#include "io_leds.hpp"

namespace app::leds
{
static io::leds::color boolToColor(const bool condition)
{
    return condition ? io::leds::color::GREEN : io::leds::color::RED;
}

void init()
{
    // TODO move this to app_leds
    std::expected<void, ErrorCode> ec = io::leds::update(io::leds::config{
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

    ec = io::leds::setBrightness(1.0);

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
    std::expected<void, ErrorCode> ec = io::leds::update(io::leds::config{
        boolToColor(app::can_rx::RSM_Heartbeat_get()),
        boolToColor(app::can_rx::BMS_Heartbeat_get()),
        boolToColor(app::can_rx::VC_Heartbeat_get()),
        boolToColor(app::can_rx::FSM_Heartbeat_get()),
        io::leds::color::GREEN,
        boolToColor(app::can_rx::DAM_Heartbeat_get()),
        boolToColor(switches::launch_control_get()),
        boolToColor(switches::start_get()),
        boolToColor(app::can_rx::VC_FirstFaultNode_get() == app::can_utils::ShutdownNode::OK),
        switches::regen_get(),
        switches::torque_vectoring_get(),
        !app::can_rx::BMS_ImdCurrentlyOk_get(),
        false,
        !app::can_rx::BMS_BspdCurrentlyOk_get(),
    });

    LOG_IF_ERR(ec);

    ec = io::leds::setBrightness(1.0);

    LOG_IF_ERR(ec);
}
} // namespace app::leds
