#include "main.h"
#include "Io_Brake.h"

bool Io_Brake_IsActuated(void)
{
    return HAL_GPIO_ReadPin(BSPD_BRAKE_STATUS_GPIO_Port, BSPD_BRAKE_STATUS_Pin) == GPIO_PIN_SET;
}
