#include "hw_gpio.h"

void hw::gpio::writePin(bool value) const
{
    HAL_GPIO_WritePin(port, pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool hw::gpio::readPin() const
{
    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET;
}
