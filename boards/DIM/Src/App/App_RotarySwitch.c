#include <stdlib.h>
#include <assert.h>
#include "App_CanMsgs.h"
#include "App_RotarySwitch.h"

static_assert(
    DRIVE_MODE1 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_1_CHOICE,
    "DRIVE_MODE1 must match its DBC signal choice");
static_assert(
    DRIVE_MODE2 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_2_CHOICE,
    "DRIVE_MODE2 must match its DBC signal choice");
static_assert(
    DRIVE_MODE3 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_3_CHOICE,
    "DRIVE_MODE3 must match its DBC signal choice");
static_assert(
    DRIVE_MODE4 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_4_CHOICE,
    "DRIVE_MODE4 must match its DBC signal choice");
static_assert(
    DRIVE_MODE5 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_5_CHOICE,
    "DRIVE_MODE5 must match its DBC signal choice");
static_assert(
    DRIVE_MODE_INVALID ==
        CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_INVALID_CHOICE,
    "DRIVE_MODE_INVALID must match its DBC signal choice");

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

void App_RotarySwitch_Destroy(struct RotarySwitch *const rotary_switch)
{
    free(rotary_switch);
}

ExitCode App_RotarySwitch_GetDriveMode(
    const struct RotarySwitch *const rotary_switch,
    enum DriveMode *const            returned_drive_mode)
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
