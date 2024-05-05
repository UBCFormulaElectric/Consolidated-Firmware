#include "io_sd.h"

static const SdGpio *sd;

void io_sdGpio_init(const SdGpio *sdGpio)
{
    sd = sdGpio;
}

bool io_sdGpio_checkSdPresent()
{
    return !hw_gpio_readPin(&sd->sd_present);
}