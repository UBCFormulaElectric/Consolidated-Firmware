#include "Io_Hal.h"
#include "main.h"
#include "Io_Charger.h"

void Io_Charger_Enable(void)
{
    HAL_GPIO_WritePin(CHRG_EN_3V3_GPIO_Port, CHRG_EN_3V3_Pin, GPIO_PIN_SET);
}

void Io_Charger_Disable(void)
{
    HAL_GPIO_WritePin(CHRG_EN_3V3_GPIO_Port, CHRG_EN_3V3_Pin, GPIO_PIN_RESET);
}

bool Io_Charger_IsConnected(void)
{
    return HAL_GPIO_ReadPin(CHRG_STATE_3V3_GPIO_Port, CHRG_STATE_3V3_Pin) == GPIO_PIN_SET;
}

bool Io_Charger_HasFaulted(void)
{
    return HAL_GPIO_ReadPin(CHRG_FLT_3V3_GPIO_Port, CHRG_FLT_3V3_Pin) == GPIO_PIN_SET;
}
