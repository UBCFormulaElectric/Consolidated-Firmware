#include <stm32f3xx.h>
#include "main.h"
#include "Io_BQ29209.h"

bool Io_BQ29209_IsOverVoltage(void)
{
    return HAL_GPIO_ReadPin(OV_FAULT_MCU_GPIO_Port, OV_FAULT_MCU_Pin) ==
           GPIO_PIN_RESET;
}
