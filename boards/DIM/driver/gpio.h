#pragma once

#include "Io_Hal.h"

typedef struct
{
    GPIO_TypeDef port;
    uint16_t     pin;
} Gpio;

void gpio_init(Gpio *gpio, GPIO_TypeDef port, uint16_t pin);
void gpio_write(Gpio *gpio, bool set);
bool gpio_read(Gpio *gpio);