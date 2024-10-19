#include "io_brake_light.h"
#include "io_led.h"

extern "C" {
#include "app_canTx.h"
}

namespace io::leds 
{

void io_brake_light_set(bool val)
{
    io::leds::brake_light.set(val)
    app_canTx_RSM_BrakeLight_set(val);
}

void io_led_set(bool val)
{
    io::leds::led.set(val)
}

}

