#include <stm32f3xx.h>
#include "main.h"
#include "Io_Latches.h"

void Io_Latches_EnableOnBmsOk(void)
{
    HAL_GPIO_WritePin(BMS_OK_GPIO_Port, BMS_OK_Pin, GPIO_PIN_SET);
}

void Io_Latches_DisableBmsOk(void)
{
    HAL_GPIO_WritePin(BMS_OK_GPIO_Port, BMS_OK_Pin, GPIO_PIN_RESET);
}

bool Io_Latches_IsBmsOkEnabled(void)
{
    return HAL_GPIO_ReadPin(BMS_OK_GPIO_Port, BMS_OK_Pin) == GPIO_PIN_SET;
}

bool Io_Latches_IsImdOkEnabled(void)
{
    return HAL_GPIO_ReadPin(IMD_OK_GPIO_Port, IMD_OK_Pin) == GPIO_PIN_SET;
}

bool Io_Latches_IsBspdOkEnabled(void)
{
    return HAL_GPIO_ReadPin(BSPD_OK_GPIO_Port, BSPD_OK_Pin) == GPIO_PIN_SET;
}
