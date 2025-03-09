#include "io_tsim.h"

void io_tsim_set_red_high(void)
{
    hw_gpio_writePin(&tsim_red_en_pin, true);
}

void io_tsim_set_red_low(void)
{
    hw_gpio_writePin(&tsim_red_en_pin, false);
}

void io_tsim_set_green_high(void)
{
    hw_gpio_writePin(&ntsim_green_en_pin, true);
}

void io_tsim_set_green_low(void)
{
    hw_gpio_writePin(&ntsim_green_en_pin, false);
}
