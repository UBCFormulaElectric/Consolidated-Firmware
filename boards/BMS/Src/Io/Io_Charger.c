#include <stm32f3xx.h>
#include "main.h"
#include "Io_Charger.h"

void Io_Charger_Enable(void)
{
    HAL_GPIO_WritePin(BRUSA_PON_GPIO_Port, BRUSA_PON_Pin, GPIO_PIN_SET);
}

void Io_Charger_Disable(void)
{
    HAL_GPIO_WritePin(BRUSA_PON_GPIO_Port, BRUSA_PON_Pin, GPIO_PIN_RESET);
}

bool Io_Charger_IsConnected(void)
{
    return HAL_GPIO_ReadPin(CHARGE_STATE_GPIO_Port, CHARGE_STATE_Pin) == GPIO_PIN_SET;
}

bool Io_Charger_HasFaulted(void)
{
    return HAL_GPIO_ReadPin(BRUSA_FLT_GPIO_Port, BRUSA_FLT_Pin) == GPIO_PIN_RESET;
}
