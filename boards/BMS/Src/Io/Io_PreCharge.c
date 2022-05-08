#include "Io_PreCharge.h"
#include "main.h"

void Io_PreCharge_Enable(void)
{
    HAL_GPIO_WritePin(PRE_CHARGE_EN_GPIO_Port, PRE_CHARGE_EN_Pin, GPIO_PIN_SET);
}

void Io_PreCharge_Disable(void)
{
    HAL_GPIO_WritePin(PRE_CHARGE_EN_GPIO_Port, PRE_CHARGE_EN_Pin, GPIO_PIN_RESET);
}
