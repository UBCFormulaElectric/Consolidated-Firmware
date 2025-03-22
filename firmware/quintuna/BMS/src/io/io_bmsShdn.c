#include "io_bmsShdn.h"
#include "hw_gpios.h"

bool io_bmsShdn_shdn_en_pin_get(void)
{
    return hw_gpio_readPin(&shdn_en_pin);
}

bool io_bmsShdn_hv_p_intlck_sns_pin_get(void)
{
    return hw_gpio_readPin(&hv_p_intlck_sns_pin);
}
