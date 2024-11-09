#include "io_brake_light.h"
#include "io_led.h"

extern "C"
{
#include "app_canTx.h"
}

namespace io::leds
{

void brake_light_set(bool val)
{
    io::binary_led::brake_light.set(val);
    app_canTx_RSM_BrakeLight_set(val);
}

void io_led_set(bool val)
{
    io::binary_led::led.set(val);
}

} // namespace io::leds
