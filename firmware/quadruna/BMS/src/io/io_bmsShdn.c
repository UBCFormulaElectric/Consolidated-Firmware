#include "io_bmsShdn.h"

static const BmsShdnConfig *shdn_config = NULL;

void io_bmsShdn_init(const BmsShdnConfig *bms_shdn_config)
{
    shdn_config = bms_shdn_config;
}

bool io_bmsShdn_TS_ILCK_OK_get(void)
{
    return hw_gpio_readPin(&shdn_config->ts_ilck_ok_gpio);
}

bool io_bmsShdn_HVD_OK_get(void)
{
    return hw_gpio_readPin(&shdn_config->hvd_ok_gpio);
}
