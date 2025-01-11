#include "hw_gpios.h"
#include "io_fans.h"

const Fans rad_fan = { .src = &rad_fan_pin };
const Fans acc_fan = { .src = &acc_fan_pin };

void io_fan_set(Fans *fans, bool on)
{
    hw_gpio_writePin(fans->src, on);
}
