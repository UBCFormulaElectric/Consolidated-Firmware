#include "io_vcShdn.h"


static const VcShdnConfig *shdn_config = NULL;

void io_vcShdn_init(const VcShdnConfig *shutdown_config)
{
    shdn_config = shutdown_config;
}

bool io_vcShdn_TsmsFault_get(void)
{
    return hw_gpio_readPin(shdn_config->tsms_gpio);
}

bool io_vcShdn_LEStopFault_get(void)
{
    return hw_gpio_readPin(shdn_config->LE_stop_gpio);
}

bool io_vcShdn_REStopFault_get(void)
{
    return hw_gpio_readPin(shdn_config->RE_stop_gpio);
}

bool io_vcShdn_SplitterBoxInterlockFault_get(void)
{
    return hw_gpio_readPin(shdn_config->splitter_box_interlock_gpio);
}
