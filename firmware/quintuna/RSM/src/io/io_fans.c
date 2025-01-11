#include "hw_gpios.h"
#include "io_fans.h"

const Fans rad_fan = { rad_fan_pin };
const Fans acc_fan = { acc_fan_pin };

void io_fan_set(Fans *fans, bool on)
{
    hw_gpio_writePin(&fans->src, on);
}