#include "Io_Buzzer.h"
#include "main.h"
#include <stm32f4xx.h>

void Io_Buzzer_TurnOn(void)
{
    HAL_GPIO_WritePin(BUZZER_EN_3V3_GPIO_Port, BUZZER_EN_3V3_Pin, GPIO_PIN_SET);
}

void Io_Buzzer_TurnOff(void)
{
    HAL_GPIO_WritePin(BUZZER_EN_3V3_GPIO_Port, BUZZER_EN_3V3_Pin, GPIO_PIN_RESET);
}
