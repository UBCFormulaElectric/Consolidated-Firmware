#include "io_fans.h"

static const Gpio *acc_fan = NULL;
static const Gpio *rad_fan = NULL;

void io_fan_init(const Gpio *acc_fan_in, const Gpio *rad_fan_in)
{
    acc_fan = acc_fan_in;
    rad_fan = rad_fan_in;
}

void io_acc_fan_set(bool on)
{
    hw_gpio_writePin(acc_fan, on);
}

void io_rad_fan_set(bool on)
{
    hw_gpio_writePin(rad_fan, on);
}
