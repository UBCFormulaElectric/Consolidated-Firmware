#include <stm32f3xx.h>
#include "main.h"
#include "Io_Buzzer.h"

void Io_Buzzer_TurnOn(void)
{
    HAL_GPIO_WritePin(BUZZER_EN_GPIO_Port, BUZZER_EN_Pin, GPIO_PIN_SET);
}

void Io_Buzzer_TurnOff(void)
{
    HAL_GPIO_WritePin(BUZZER_EN_GPIO_Port, BUZZER_EN_Pin, GPIO_PIN_RESET);
}
