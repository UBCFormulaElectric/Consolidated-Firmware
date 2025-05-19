#include "io_sd.h"
#include "hw_sds.h"

bool io_sdGpio_checkSdPresent()
{
    return !hw_gpio_readPin(sd1.present_gpio);
}