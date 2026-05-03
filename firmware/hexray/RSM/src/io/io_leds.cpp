#include "io_leds.hpp"
#include "hw_gpios.hpp"

namespace io
{
const binary_led debug_led(led_pin);
const binary_led brake_light(brake_light_en);
} // namespace io
