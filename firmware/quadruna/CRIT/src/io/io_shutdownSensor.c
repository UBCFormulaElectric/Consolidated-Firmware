#include "io_shutdownSensor.h"
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stm32f4xx_hal.h>

static const ShutdownSensor *config;

bool io_shutdownSensor_readPin(const ShutdownSensor *gpio_pin)
{
    return hw_gpio_readPin(gpio_pin->shdn_sen_pin);
}
