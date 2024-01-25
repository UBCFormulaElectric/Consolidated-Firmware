#include "Io_SharedGpio.h"

void Io_SharedGpio_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t User_PinState)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, User_PinState);
}
