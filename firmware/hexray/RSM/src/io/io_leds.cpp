#include "io_leds.hpp"
#include "hw_gpios.hpp"

namespace io
{
    const binary_led led(hw::gpio::led);
    const binary_led brake_light(hw::gpio::brake_light_en);
}

