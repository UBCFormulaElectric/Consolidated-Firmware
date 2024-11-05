#pragma once
#include "io_led.h"
#include "hw_gpios.h"

namespace io::leds
{

void brake_light_set(bool val);

const io::binary_led brake_light{ hw::gpio::brake_light_en_pin };
const io::binary_led led{ hw::gpio::led_pin };

} // namespace io::leds
