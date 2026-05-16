#include "app_canRx.hpp"
#include "app_switches.hpp"
#include "app_leds.hpp"
#include "io_leds.hpp"

namespace app::leds
{
// static io::leds::color boolToColor(const bool condition)
// {
//     return condition ? io::leds::color::GREEN : io::leds::color::RED;
// }

void init()
{
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
    std::expected<void, ErrorCode> ec = io::leds::update(io::leds::config{
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        io::leds::color::GREEN,
        true,
        true,
        true,
        true,
        true,
    });

    LOG_IF_ERR(ec);

    ec = io::leds::setBrightness(0.01f);

    LOG_IF_ERR(ec);
}
} // namespace app::leds
