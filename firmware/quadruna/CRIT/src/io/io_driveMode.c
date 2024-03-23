#include "io_driveMode.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stm32f4xx_hal.h>

static const DriveMode *config;

uint16_t io_driveMode_readPins()
{
    uint16_t out = (uint16_t)(hw_gpio_readPin(config->n_drive_mode_0_pin));
    out += (uint16_t)(hw_gpio_readPin(config->n_drive_mode_1_pin) << 1);
    out += (uint16_t)(hw_gpio_readPin(config->n_drive_mode_2_pin) << 2);
    out += (uint16_t)(hw_gpio_readPin(config->n_drive_mode_3_pin) << 3);
    return out;
}
