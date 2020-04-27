#pragma once

#include "App_ErrorCode.h"

enum DriveMode
{
    DRIVE_MODE1,
    DRIVE_MODE2,
    DRIVE_MODE3,
    DRIVE_MODE4,
    DRIVE_MODE5,
    NUM_DRIVE_MODE,
    DRIVE_MODE_INVALID = NUM_DRIVE_MODE,
};

struct RotarySwitch;

struct RotarySwitch *
    App_RotarySwitch_Create(enum DriveMode (*get_drive_mode)(void));

void App_RotarySwitch_Destroy(struct RotarySwitch *rotary_switch);

ExitCode App_RotarySwitch_GetDriveMode(
    const struct RotarySwitch *rotary_switch,
    enum DriveMode *           returned_drive_mode);
