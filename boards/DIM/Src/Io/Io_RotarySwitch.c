#include "main.h"
#include "Io_RotarySwitch.h"

static enum DriveMode drive_mode = DRIVE_MODE_INVALID;

void Io_RotarySwitch_Init(void)
{
    if (HAL_GPIO_ReadPin(DRIVE_MODE1_GPIO_Port, DRIVE_MODE1_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE1;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE2_GPIO_Port, DRIVE_MODE2_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE2;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE3_GPIO_Port, DRIVE_MODE3_Pin) ==
             GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE3;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE4_GPIO_Port, DRIVE_MODE4_Pin) ==
             GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE4;
    }
    else if (HAL_GPIO_ReadPin(DRIVE_MODE5_GPIO_Port, DRIVE_MODE5_Pin) ==
             GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE5;
    }
    else
    {
        drive_mode = DRIVE_MODE_INVALID;
    }
}

void Io_RotarySwitch_ExtiCallback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case DRIVE_MODE1_Pin:
        {
            drive_mode = DRIVE_MODE1;
        }
        break;
        case DRIVE_MODE2_Pin:
        {
            drive_mode = DRIVE_MODE2;
        }
        break;
        case DRIVE_MODE3_Pin:
        {
            drive_mode = DRIVE_MODE3;
        }
        break;
        case DRIVE_MODE4_Pin:
        {
            drive_mode = DRIVE_MODE4;
        }
        break;
        case DRIVE_MODE5_Pin:
        {
            drive_mode = DRIVE_MODE5;
        }
        break;
        default:
        {
            drive_mode = DRIVE_MODE_INVALID;
        }
        break;
    }
}

enum DriveMode Io_RotarySwitch_GetDriveMode(void)
{
    return drive_mode;
}
