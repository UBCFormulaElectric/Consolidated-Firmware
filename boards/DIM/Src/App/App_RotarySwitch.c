#include <stdlib.h>
#include <assert.h>
#include "App_RotarySwitch.h"

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
