#include <stm32f3xx.h>
#include "main.h"
#include "Io_InverterSwitches.h"

void Io_InverterSwitches_TurnOnLeft()
{
    HAL_GPIO_WritePin(INVERTER_L_EN_GPIO_Port, INVERTER_L_EN_Pin, GPIO_PIN_SET);
}

void Io_InverterSwitches_TurnOffLeft()
{
    HAL_GPIO_WritePin(INVERTER_L_EN_GPIO_Port, INVERTER_L_EN_Pin, GPIO_PIN_RESET);
}

void Io_InverterSwitches_TurnOnRight()
{
    HAL_GPIO_WritePin(INVERTER_R_EN_GPIO_Port, INVERTER_R_EN_Pin, GPIO_PIN_SET);
}

void Io_InverterSwitches_TurnOffRight()
{
    HAL_GPIO_WritePin(INVERTER_R_EN_GPIO_Port, INVERTER_R_EN_Pin, GPIO_PIN_RESET);
}

bool Io_InverterSwitches_IsRightInverterOn()
{
    return HAL_GPIO_ReadPin(INVERTER_R_EN_GPIO_Port, INVERTER_R_EN_Pin) == GPIO_PIN_SET;
}

bool Io_InverterSwitches_IsLeftInverterOn()
{
    return HAL_GPIO_ReadPin(INVERTER_L_EN_GPIO_Port, INVERTER_L_EN_Pin) == GPIO_PIN_SET;
}
