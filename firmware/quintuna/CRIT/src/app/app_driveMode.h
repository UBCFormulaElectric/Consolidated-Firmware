#pragma once

typedef enum
{
    DRIVE_MODE_INDOOR = 0u,
    DRIVE_MODE_OUTDOOR_SPEED_LIMIT,
    DRIVE_MODE_VC,
    DRIVE_MODE_DRIVER_ASSIST,
    DRIVE_MODE_DEBUG,
    DRIVE_MODE_REVERSE,
    DRIVE_MODE_LAUNCH_CONTROL
} DriveMode_e;

void app_driveMode_broadcast(void);
