#include <stdlib.h>
#include <assert.h>
#include "App_DriveMode.h"

struct RotarySwitch
{
    enum DriveMode (*get_drive_mode)(void);
};

struct RotarySwitch *
    App_RotarySwitch_Create(enum DriveMode (*const get_drive_mode)(void))
{
    struct RotarySwitch *rotary_switch = malloc(sizeof(struct RotarySwitch));
    assert(rotary_switch != NULL);

    rotary_switch->get_drive_mode = get_drive_mode;

    return rotary_switch;
}

void App_RotarySwitch_Destroy(struct RotarySwitch *rotary_switch)
{
    free(rotary_switch);
}

ExitCode App_RotarySwitch_GetDriveMode(
    const struct RotarySwitch *rotary_switch,
    enum DriveMode *           returned_drive_mode)
{
    enum DriveMode drive_mode = rotary_switch->get_drive_mode();
    ExitCode       exit_code  = EXIT_CODE_OK;

    if (drive_mode >= NUM_DRIVE_MODE)
    {
        exit_code = EXIT_CODE_OUT_OF_RANGE;
    }

    *returned_drive_mode = drive_mode;

    return exit_code;
}
