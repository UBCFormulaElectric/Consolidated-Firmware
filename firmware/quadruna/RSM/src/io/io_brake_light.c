#include "io_brake_light.h"

static const BinaryLed *brake_light = NULL;

void io_brake_light_init(const BinaryLed *brake_light_in)
{
    brake_light = brake_light_in;
}

void io_brake_light_set(bool val)
{
    io_led_enable(brake_light, val);
}
