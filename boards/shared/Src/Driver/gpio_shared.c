#include "main.h"
#include "gpio_cfg.h"
#include "gpio_shared.h"

void gpio_shared_write(gpio_e gpo, bool is_gpo_set)
{
    if (gpio_cfg->is_gpo)
    {
        HAL_GPIO_WritePin(gpio_cfg[gpo].port, gpio_cfg[gpo].pin, (GPIO_PinState)is_gpo_set);
    }
}

bool gpio_shared_read(gpio_e gpo)
{
    return HAL_GPIO_ReadPin(gpio_cfg[gpo].port, gpio_cfg[gpo].pin) == GPIO_PIN_SET;
}
