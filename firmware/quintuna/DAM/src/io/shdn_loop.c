#include "io/shdn_loop.h"
#include "hw/gpios.h"

bool io_r_shdn_pin_is_high(void)
{
    return hw_gpio_readPin(&r_shdn_sense_pin);
}

bool io_l_shdn_pin_is_high(void)
{
    return hw_gpio_readPin(&l_shdn_sense_pin);
}
