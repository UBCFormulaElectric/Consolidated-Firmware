#include <stdlib.h>
#include <assert.h>
#include "App_RegenPaddle.h"

struct RegenPaddle
{
    float (*get_paddle_position)(void);
    float lower_deadzone;
    float upper_deadzone;
    float regen_output;
};

struct RegenPaddle *App_RegenPaddle_Create(
    float (*const get_pedal_position)(void),
    float lower_deadzone,
    float upper_deadzone)
{
    struct RegenPaddle *regen_paddle = malloc(sizeof(struct RegenPaddle));

    assert(regen_paddle != NULL);

    regen_paddle->get_paddle_position = get_pedal_position;
    regen_paddle->lower_deadzone      = lower_deadzone;
    regen_paddle->upper_deadzone      = upper_deadzone;

    return regen_paddle;
}

void App_RegenPaddle_Destroy(struct RegenPaddle *const regen_paddle)
{
    free(regen_paddle);
}

void App_RegenPaddle_Tick(struct RegenPaddle *const regen_paddle)
{
    const float paddle_pressed = regen_paddle->get_paddle_position();

    if (paddle_pressed > regen_paddle->upper_deadzone)
    {
        regen_paddle->regen_output = 100.0f;
    }
    else if (paddle_pressed < regen_paddle->lower_deadzone)
    {
        regen_paddle->regen_output = 0.0f;
    }
    else
    {
        regen_paddle->regen_output =
            (paddle_pressed - regen_paddle->lower_deadzone) /
            (regen_paddle->upper_deadzone - regen_paddle->lower_deadzone) *
            100.0f;
    }
}

float App_RegenPaddle_GetRegen(const struct RegenPaddle *const regen_paddle)
{
    return regen_paddle->regen_output;
}
