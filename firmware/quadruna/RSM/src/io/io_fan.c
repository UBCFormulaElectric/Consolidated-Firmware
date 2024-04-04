#include "io_fan.h"

void io_fan_enable(const BinaryFan *fan, bool on)
{
    hw_gpio_writePin(&fan->gpio, on);
}