#include "io_tsim.h"
#include "hw_gpios.h"

void io_tsim_set_red(bool state)
{
    hw_gpio_writePin(&tsim_red_en_pin, state);
}
void io_tsim_set_green(bool state)
{
    hw_gpio_writePin(&ntsim_green_en_pin, state);
}
