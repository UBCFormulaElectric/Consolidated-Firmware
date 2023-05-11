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

void Io_Leds_TurnOnShdnLed(void)
{
    HAL_GPIO_WritePin(SHDN_LED_GPIO_Port, SHDN_LED_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffShdnLed(void)
{
    HAL_GPIO_WritePin(SHDN_LED_GPIO_Port, SHDN_LED_Pin, GPIO_PIN_RESET);
}

void Io_Leds_TurnOnDriveLed(void)
{
    HAL_GPIO_WritePin(IGNTN_LED_GPIO_Port, IGNTN_LED_Pin, GPIO_PIN_SET);
}

void Io_Leds_TurnOffDriveLed(void)
{
    HAL_GPIO_WritePin(IGNTN_LED_GPIO_Port, IGNTN_LED_Pin, GPIO_PIN_RESET);
}
