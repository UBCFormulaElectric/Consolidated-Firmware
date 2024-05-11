#include "io_fans.h"
#include "app_canTx.h"

static const Gpio *acc_fan;
static const Gpio *rad_fan;

void io_fans_init(const Gpio *acc_fan_in, const Gpio *rad_fan_in)
{
    acc_fan = acc_fan_in;
    rad_fan = rad_fan_in;
}

void io_fans_accfan_set(bool val)
{
    hw_gpio_writePin(acc_fan, val);
    app_canTx_RSM_AccumulatorFan_set(val);
}
void io_fans_radfan_set(bool val)
{
    hw_gpio_writePin(rad_fan, val);
    app_canTx_RSM_RadiatorFan_set(val);
}
