#include "io/bmsShdn.h"
#include "hw/gpios.h"

bool io_bmsShdn_msd_shdn_sns_pin_get(void)
{
    return hw_gpio_readPin(&msd_shdn_sns_pin);
}

bool io_bmsShdn_hv_p_intlck_sns_pin_get(void)
{
    return hw_gpio_readPin(&hv_p_intlck_sns_pin);
}

bool io_bmsShdn_hv_n_intlck_sns_pin_get(void)
{
    return hw_gpio_readPin(&hv_n_intlck_sns_pin);
}
