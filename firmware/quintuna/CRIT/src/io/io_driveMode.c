#include "io_driveMode.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stm32f4xx_hal.h>

static const DriveMode *config = NULL;

void io_driveMode_init(const DriveMode *config_in)
{
    config = config_in;
}

uint16_t io_driveMode_readPins(void)
{
    uint16_t out = (uint16_t)(hw_gpio_readPin(config->regen));
    out += (uint16_t)(hw_gpio_readPin(config->launch_control) << 1);
    out += (uint16_t)(hw_gpio_readPin(config->torque_vectoring) << 2);
    return out;
}
