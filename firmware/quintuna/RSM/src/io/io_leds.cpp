#include "io_brake_light.h"
#include "io_led.h"
#include "io_leds.h"

extern "C"
{
#include "app_canTx.h"
}

namespace io::leds
{

const io::binary_led brake_light{ hw::gpio::brake_light_en_pin };
const io::binary_led led{ hw::gpio::led_pin };

} // namespace io::leds
