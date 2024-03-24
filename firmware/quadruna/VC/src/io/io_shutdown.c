#include <stddef.h>
#include "io_shutdown.h"

static const ShutdownConfig *config;

void io_shutdown_init(const ShutdownConfig *shutdown_config)
{
    config = shutdown_config;
}

bool io_shutdown_hasTsmsFault()
{
    return hw_gpio_readPin(&config->tsms_gpio);
}

bool io_shutdown_hasPcmFault()
{
    return hw_gpio_readPin(&config->pcm_gpio);
}

bool io_shutdown_hasLEStopFault()
{
    return hw_gpio_readPin(&config->RE_stop_gpio);
}

bool io_shutdown_hasREStopFault()
{
    return hw_gpio_readPin(&config->LE_stop_gpio);
}


bool io_shutdown_hasSplitterBoxInterlockFault()
{
    return hw_gpio_readPin(&config->splitter_box_interlock_gpio);
}
