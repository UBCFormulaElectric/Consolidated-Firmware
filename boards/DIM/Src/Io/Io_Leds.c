#include <stm32f4xx.h>
#include "main.h"
#include "Io_Leds.h"

void Io_Leds_TurnOnImdLed(void)
{
    HAL_GPIO_WritePin(IMD_LED_GPIO_Port, IMD_LED_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffImdLed(void)
{
    HAL_GPIO_WritePin(IMD_LED_GPIO_Port, IMD_LED_Pin, GPIO_PIN_RESET);
}

void Io_Leds_TurnOnBspdLed(void)
{
    HAL_GPIO_WritePin(BSPD_LED_GPIO_Port, BSPD_LED_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffBspdLed(void)
{
    HAL_GPIO_WritePin(BSPD_LED_GPIO_Port, BSPD_LED_Pin, GPIO_PIN_RESET);
}
