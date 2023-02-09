#include <limits.h>
#include "main.h"
#include "Io_DriveModeSwitch.h"

uint32_t Io_DriveModeSwitch_GetPosition(void)
{
    // TODO: This is untested
    uint32_t position = UINT_MAX;

    if (HAL_GPIO_ReadPin(DRIVE_MODE_0_GPIO_Port, DRIVE_MODE_0_Pin) == GPIO_PIN_SET)
    {
        position = 0;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE_1_GPIO_Port, DRIVE_MODE_1_Pin) == GPIO_PIN_SET)
    {
        position = 1;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE_2_GPIO_Port, DRIVE_MODE_2_Pin) == GPIO_PIN_SET)
    {
        position = 2;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE_3_GPIO_Port, DRIVE_MODE_3_Pin) == GPIO_PIN_SET)
    {
        position = 3;
    }
    else
    {
        position = 4;
    }

    return position;
}
