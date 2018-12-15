/******************************************************************************
 * Includes
 *****************************************************************************/
#include "SharedGpio.h"

void SharedGpio_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t PinState)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}
