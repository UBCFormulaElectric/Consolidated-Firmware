#include <stm32f4xx.h>
#include "main.h"
#include "Io_BrakeLight.h"

void Io_BrakeLight_TurnOn(void)
{
    HAL_GPIO_WritePin(BRAKE_LIGHT_EN_3V3_GPIO_Port, BRAKE_LIGHT_EN_3V3_Pin, GPIO_PIN_SET);
}

void Io_BrakeLight_TurnOff(void)
{
    HAL_GPIO_WritePin(BRAKE_LIGHT_EN_3V3_GPIO_Port, BRAKE_LIGHT_EN_3V3_Pin, GPIO_PIN_RESET);
}
