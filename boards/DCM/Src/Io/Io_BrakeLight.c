#include <stm32f3xx.h>
#include "main.h"
#include "Io_BrakeLight.h"

void Io_BrakeLight_TurnOn(void)
{
    HAL_GPIO_WritePin(BRAKE_LIGHT_EN_GPIO_Port, BRAKE_LIGHT_EN_Pin, GPIO_PIN_SET);
}

void Io_BrakeLight_TurnOff(void)
{
    HAL_GPIO_WritePin(BRAKE_LIGHT_EN_GPIO_Port, BRAKE_LIGHT_EN_Pin, GPIO_PIN_RESET);
}
