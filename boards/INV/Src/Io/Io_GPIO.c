#include "Io_GPIO.h"
#include "main.h"

bool Io_GPIO_GetMotorOTAlarm(void)
{
    return HAL_GPIO_ReadPin(nMOD_OT_ALARM_GPIO_Port, nMOD_OT_ALARM_Pin);
}

void Io_GPIO_SetGPIOD1(bool value)
{
    if(value)
    {
        HAL_GPIO_WritePin(GPIOD_1_GPIO_Port, GPIOD_1_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOD_1_GPIO_Port, GPIOD_1_Pin, GPIO_PIN_RESET);
    }
}

bool Io_GPIOD_ReadGPIO1(void)
{
    return HAL_GPIO_ReadPin(GPIOD_1_GPIO_Port, GPIOD_1_Pin);
}

bool Io_GPIOD_ReadGPIO2(void)
{
    return HAL_GPIO_ReadPin(GPIOD_2_GPIO_Port, GPIOD_2_Pin);
}
