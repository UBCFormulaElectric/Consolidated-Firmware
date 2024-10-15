#pragma once
#include "io_led.h"
#include "hw_gpios.h"

namespace io::leds
{
const io::binary_led imd_led{ hw::gpio::led };
} // namespace io::leds