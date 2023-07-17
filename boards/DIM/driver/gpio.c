#include "gpio.h"

void gpio_init(Gpio *const gpio, GPIO_TypeDef port, uint16_t pin)
{
    gpio->port = port;
    gpio->pin = pin;
}

void gpio_write(Gpio* gpio, bool set)
{
    HAL_GPIO_WritePin(gpio->port, gpio->pin, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool gpio_read(Gpio* gpio)
{
    return HAL_GPIO_ReadPin(gpio->port, gpio->pin) == GPIO_PIN_SETs;
}