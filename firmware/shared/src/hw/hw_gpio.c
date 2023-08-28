#include "hw_gpio.h"
#include "hw_stm32Hal.h"

void hw_gpio_writePin(const Gpio *gpio, bool state)
{
    HAL_GPIO_WritePin(gpio->port, gpio->pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool hw_gpio_readPin(const Gpio *gpio)
{
    return HAL_GPIO_ReadPin(gpio->port, gpio->pin) == GPIO_PIN_SET;
}
