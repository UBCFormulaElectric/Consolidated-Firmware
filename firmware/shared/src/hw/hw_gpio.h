#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"

typedef struct
{
    GPIO_TypeDef* const port;
    const uint16_t      pin;
} Gpio;

/**
 * Write a GPIO pin high or low.
 * @param gpio The GPIO pin instance.
 * @param state True to set pin high, false to set pin low.
 */
void hw_gpio_writePin(const Gpio* gpio, bool state);

/**
 * Read the state of a GPIO pin.
 * @param gpio The GPIO pin instance.
 * @return True if pin is high, false if pin is low.
 */
bool hw_gpio_readPin(const Gpio* gpio);