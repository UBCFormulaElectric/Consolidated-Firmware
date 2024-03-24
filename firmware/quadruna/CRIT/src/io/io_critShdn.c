#include "io_critShdn.h"

static const CritShdnConfig *shdn_config;

void io_critShdn_init(const CritShdnConfig *crit_shdn_config)
{
    shdn_config = crit_shdn_config;
}

int io_crit_num_shdn_nodes()
{
    return shdn_config->crit_num_nodes;
}

bool io_get_SHDN_SEN_OK()
{
    return hw_gpio_readPin(&shdn_config->shdn_sen_ok_gpio);
}

bool io_get_INERTIA_SEN_OK()
{
    return hw_gpio_readPin(&shdn_config->inertia_sen_ok_gpio);
}