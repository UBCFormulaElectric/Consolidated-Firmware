#include <limits.h>
#include "main.h"
#include "Io_DriveModeSwitch.h"

uint32_t Io_DriveModeSwitch_GetPosition(void)
{
    uint32_t position = UINT_MAX;

    // The DRIVE_MODE pins are active high
    if (HAL_GPIO_ReadPin(DRIVE_MODE1_GPIO_Port, DRIVE_MODE1_Pin) == GPIO_PIN_SET)
    {
        position = 0;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE2_GPIO_Port, DRIVE_MODE2_Pin) == GPIO_PIN_SET)
    {
        position = 1;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE3_GPIO_Port, DRIVE_MODE3_Pin) == GPIO_PIN_SET)
    {
        position = 2;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE4_GPIO_Port, DRIVE_MODE4_Pin) == GPIO_PIN_SET)
    {
        position = 3;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE5_GPIO_Port, DRIVE_MODE5_Pin) == GPIO_PIN_SET)
    {
        position = 4;
    }
    else
    {
        // Pin 6 on the rotary switch is disconnected. If we rotate the rotary
        // switch to the 6th position, then all other pins on the switch will
        // read as low.
        position = 5;
    }

    return position;
}
