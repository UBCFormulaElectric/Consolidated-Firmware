#include "io_bmsShdn.h"
#include "hw_gpios.h"

bool io_bmsShdn_TS_ILCK_OK_get(void)
{
    return hw_gpio_readPin(&ts_ilck_shdn_pin);
}

bool io_bmsShdn_HVD_OK_get(void)
{
    return hw_gpio_readPin(&hvd_ok_shdn_pin);
}
