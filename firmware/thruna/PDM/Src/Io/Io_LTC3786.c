#include "hw_hal.h"
#include "main.h"
#include "Io_LTC3786.h"

bool Io_LTC3786_HasFault(void)
{
    return HAL_GPIO_ReadPin(PGOOD_GPIO_Port, PGOOD_Pin) == GPIO_PIN_RESET;
}
