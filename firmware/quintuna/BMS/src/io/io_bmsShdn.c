#include "io_bmsShdn.h"
#include "hw_gpios.h"

bool io_bmsShdn_TS_ILCK_OK_get(void)
{
    return hw_gpio_readPin(&shdn_en_pin);
}

bool io_bmsShdn_HVD_OK_get(void)
{
    return hw_gpio_readPin(&hv_p_intlck_sns_pin);
}
