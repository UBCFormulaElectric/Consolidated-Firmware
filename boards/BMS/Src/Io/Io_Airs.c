#include "Io_Adc.h"
#include "Io_Airs.h"
#include "main.h"

bool Io_Airs_IsAirNegativeClosed(void)
{
    return HAL_GPIO_ReadPin(AIR_POWER_STATUS_GPIO_Port, AIR_POWER_STATUS_Pin);
}

bool Io_Airs_IsAirPositiveClosed(void)
{
    return HAL_GPIO_ReadPin(AIR_EN_GPIO_Port, AIR_EN_Pin) == GPIO_PIN_SET;
}

void Io_Airs_CloseAirPositive(void)
{
    HAL_GPIO_WritePin(AIR_EN_GPIO_Port, AIR_EN_Pin, GPIO_PIN_SET);
}

void Io_Airs_OpenAirPositive(void)
{
    HAL_GPIO_WritePin(AIR_EN_GPIO_Port, AIR_EN_Pin, GPIO_PIN_RESET);
}
