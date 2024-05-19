#include "io_tsms.h"

static const TSMSConfig *config = NULL;

void io_tsms_init(const TSMSConfig *const in_config)
{
    config = in_config;
}

bool io_tsms_read(void)
{
    return hw_gpio_readPin(config->tsms_gpio);
}
