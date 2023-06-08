#include <limits.h>
#include "main.h"
#include "Io_DriveModeSwitch.h"
#include "stdbool.h"
// decode the binary pins back to a value from 1-12
uint32_t Io_DriveModeSwitch_GetPosition(void)
{
    // TODO: This is untested
    uint32_t position = 0;
    // drive mode gpio port is a 4 bit binary number



    if (HAL_GPIO_ReadPin(DRIVE_MODE_0_GPIO_Port, DRIVE_MODE_0_Pin) == GPIO_PIN_RESET)
    {
        position += 1;
    }
    if (HAL_GPIO_ReadPin(DRIVE_MODE_1_GPIO_Port, DRIVE_MODE_1_Pin) == GPIO_PIN_RESET)
    {
        position += 2;
    }
    if (HAL_GPIO_ReadPin(DRIVE_MODE_2_GPIO_Port, DRIVE_MODE_2_Pin) == GPIO_PIN_RESET)
    {
        position += 4;
    }
    if (HAL_GPIO_ReadPin(DRIVE_MODE_3_GPIO_Port, DRIVE_MODE_3_Pin) == GPIO_PIN_RESET)
    {
        position += 8;
    }

    return position;
}
