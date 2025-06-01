#include "io_irs.h"

#include "hw_gpios.h"

bool io_irs_isNegativeClosed(void)
{
    return hw_gpio_readPin(&msd_shdn_sns_pin);
}

void io_irs_closePositive(void)
{
    hw_gpio_writePin(&ir_p_en_pin, true);
}

void io_irs_openPositive(void)
{
    hw_gpio_writePin(&ir_p_en_pin, false);
}

bool io_irs_isPositiveClosed(void)
{
    return hw_gpio_readPin(&ir_p_en_pin);
}

void io_irs_closePrecharge(void)
{
    hw_gpio_writePin(&precharge_en_pin, true);
}

void io_irs_openPrecharge(void)
{
    hw_gpio_writePin(&precharge_en_pin, false);
}

bool io_irs_isPrechargeClosed(void)
{
    return hw_gpio_readPin(&precharge_en_pin);
}
