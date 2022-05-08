#include <stm32f3xx.h>
#include "main.h"
#include "Io_LT3650.h"

bool Io_LT3650_HasFault(void)
{
    return HAL_GPIO_ReadPin(CHRG_FAULT_GPIO_Port, CHRG_FAULT_Pin) == GPIO_PIN_RESET;
}
