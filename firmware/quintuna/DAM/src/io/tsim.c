#include "io/tsim.h"
#include "hw/gpios.h"

void io_tsim_set_off()
{
    hw_gpio_writePin(&tsim_red_en_pin, false);
    hw_gpio_writePin(&ntsim_green_en_pin, false);
}

void io_tsim_set_red()
{
    hw_gpio_writePin(&tsim_red_en_pin, true);
    hw_gpio_writePin(&ntsim_green_en_pin, false);
}
void io_tsim_set_green()
{
    hw_gpio_writePin(&ntsim_green_en_pin, true);
    hw_gpio_writePin(&tsim_red_en_pin, false);
}
