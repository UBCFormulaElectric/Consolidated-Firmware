#include "main.h"
#include "Io_RotarySwitch.h"

enum DriveMode Io_RotarySwitch_GetDriveMode(void)
{
    enum DriveMode drive_mode = DRIVE_MODE_INVALID;

    if (HAL_GPIO_ReadPin(DRIVE_MODE1_GPIO_Port, DRIVE_MODE1_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE1;
    }
    else if (
        HAL_GPIO_ReadPin(DRIVE_MODE2_GPIO_Port, DRIVE_MODE2_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE2;
    }
    else if (
        HAL_GPIO_ReadPin(DRIVE_MODE3_GPIO_Port, DRIVE_MODE3_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE3;
    }
    else if (
        HAL_GPIO_ReadPin(DRIVE_MODE4_GPIO_Port, DRIVE_MODE4_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE4;
    }
    else if (
        HAL_GPIO_ReadPin(DRIVE_MODE5_GPIO_Port, DRIVE_MODE5_Pin) ==
        GPIO_PIN_SET)
    {
        drive_mode = DRIVE_MODE5;
    }
    else
    {
        // The 6th pin on the rotary switch is disconnected. If we rotate the
        // rotary switch to the 6th pin, then Drive Mode 1-5 would all be low.
        // We treat this as an invalid drive mode.
        drive_mode = DRIVE_MODE_INVALID;
    }

    return drive_mode;
}
