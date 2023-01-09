#include <stm32f4xx.h>
#include "main.h"
#include "Io_Switches.h"

bool Io_Switches_StartSwitchIsTurnedOn(void)
{
    //    TODO: IO -> return HAL_GPIO_ReadPin(IGNTN_GPIO_Port, IGNTN_Pin) == GPIO_PIN_SET;
    return false;
}

bool Io_Switches_TractionControlSwitchIsTurnedOn(void)
{
    //    TODO: IO -> return HAL_GPIO_ReadPin(TRAC_CTRL_GPIO_Port, TRAC_CTRL_Pin) == GPIO_PIN_SET;
    return false;
}

bool Io_Switches_TorqueVectoringSwitchIsTurnedOn(void)
{
    //    TODO: IO -> return HAL_GPIO_ReadPin(TORQ_VECT_GPIO_Port, TORQ_VECT_Pin) == GPIO_PIN_SET;
    return false;
}
