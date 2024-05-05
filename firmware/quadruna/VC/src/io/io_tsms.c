#include "io_tsms.h"

static const TSMSConfig *config;

void io_tsms_init(const TSMSConfig *const in_config)
{
    config = in_config;
}

bool io_tsms_read()
{
    return hw_gpio_readPin(config->tsms_gpio);
}