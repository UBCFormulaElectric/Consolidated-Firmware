#include "hw_hal.h"
#include "main.h"
#include "Io_LT3650.h"

bool Io_LT3650_HasFault(void)
{
    return HAL_GPIO_ReadPin(N_CHRG_FAULT_GPIO_Port, N_CHRG_FAULT_Pin) == GPIO_PIN_RESET;
}
