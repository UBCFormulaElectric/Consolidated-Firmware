#include "io_vcShdn.h"

static const VcShdnConfig *config;

void io_vcShdn_init(const VcShdnConfig *shutdown_config)
{
    config = shutdown_config;
}

bool io_vcShdn_TsmsFault_get(void)
{
    return hw_gpio_readPin(&config->tsms_gpio);
}

bool io_vcShdn_PcmFault_get(void)
{
    return !hw_gpio_readPin(&config->npcm_gpio);
}

bool io_vcShdn_LEStopFault_get(void)
{
    return hw_gpio_readPin(&config->RE_stop_gpio);
}

bool io_vcShdn_REStopFault_get(void)
{
    return hw_gpio_readPin(&config->LE_stop_gpio);
}

bool io_vcShdn_SplitterBoxInterlockFault_get(void)
{
    return hw_gpio_readPin(&config->splitter_box_interlock_gpio);
}
