#include "hw/gpio.h"

void hw_gpio_writePin(const Gpio *gpio, bool state)
{
    HAL_GPIO_WritePin(gpio->port, gpio->pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void hw_gpio_togglePin(const Gpio *gpio)
{
    HAL_GPIO_TogglePin(gpio->port, gpio->pin);
}

bool hw_gpio_readPin(const Gpio *gpio)
{
    return HAL_GPIO_ReadPin(gpio->port, gpio->pin) == GPIO_PIN_SET;
}