#include "main.h"
#include "Io_DriveMode.h"

static enum DriveMode drive_mode = DRIVE_MODE_INVALID;

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
