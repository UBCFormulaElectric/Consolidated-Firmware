#include <stm32f3xx.h>
#include "main.h"
#include "Io_Leds.h"

void Io_Leds_TurnOnBmsOk(void)
{
    HAL_GPIO_WritePin(BMS_OK_GPIO_Port, BMS_OK_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffBmsOk(void)
{
    HAL_GPIO_WritePin(BMS_OK_GPIO_Port, BMS_OK_Pin, GPIO_PIN_RESET);
}

void Io_Leds_TurnOnImdOk(void)
{
    HAL_GPIO_WritePin(IMD_OK_GPIO_Port, IMD_OK_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffImdOk(void)
{
    HAL_GPIO_WritePin(IMD_OK_GPIO_Port, IMD_OK_Pin, GPIO_PIN_RESET);
}

void Io_Leds_TurnOnBspdOk(void)
{
    HAL_GPIO_WritePin(BSPD_OK_GPIO_Port, BSPD_OK_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffBspdOk(void)
{
    HAL_GPIO_WritePin(BSPD_OK_GPIO_Port, BSPD_OK_Pin, GPIO_PIN_RESET);
}
