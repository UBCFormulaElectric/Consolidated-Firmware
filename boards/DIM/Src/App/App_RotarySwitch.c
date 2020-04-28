#include <stdlib.h>
#include <assert.h>
#include "App_RotarySwitch.h"

// static_assert(
//    DRIVE_MODE1 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_1_CHOICE,
//    "DRIVE_MODE1 must match its DBC signal choice");
// static_assert(
//    DRIVE_MODE2 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_2_CHOICE,
//    "DRIVE_MODE2 must match its DBC signal choice");
// static_assert(
//    DRIVE_MODE3 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_3_CHOICE,
//    "DRIVE_MODE3 must match its DBC signal choice");
// static_assert(
//    DRIVE_MODE4 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_4_CHOICE,
//    "DRIVE_MODE4 must match its DBC signal choice");
// static_assert(
//    DRIVE_MODE5 == CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_5_CHOICE,
//    "DRIVE_MODE5 must match its DBC signal choice");
// static_assert(
//    DRIVE_MODE_INVALID ==
//        CANMSGS_DIM_ROTARY_SWITCH_DRIVE_MODE_DRIVE_MODE_INVALID_CHOICE,
//    "DRIVE_MODE_INVALID must match its DBC signal choice");

struct RotarySwitch
{
    uint32_t (*get_position)(void);
    uint32_t num_positions;
};

struct RotarySwitch *App_RotarySwitch_Create(
    uint32_t (*const get_position)(void),
    uint32_t num_positions)
{
    struct RotarySwitch *rotary_switch = malloc(sizeof(struct RotarySwitch));
    assert(rotary_switch != NULL);

    rotary_switch->get_position  = get_position;
    rotary_switch->num_positions = num_positions;

    return rotary_switch;
}

void App_RotarySwitch_Destroy(struct RotarySwitch *const rotary_switch)
{
    free(rotary_switch);
}

ExitCode App_RotarySwitch_GetPosition(
    const struct RotarySwitch *const rotary_switch,
    uint32_t *const                  returned_position)
{
    uint32_t position = rotary_switch->get_position();

    ExitCode exit_code = EXIT_CODE_OK;

    if (position >= rotary_switch->num_positions)
    {
        exit_code = EXIT_CODE_OUT_OF_RANGE;
    }

    *returned_position = position;

    return exit_code;
}
