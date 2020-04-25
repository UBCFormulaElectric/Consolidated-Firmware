#include <stdlib.h>
#include <assert.h>
#include "App_RegenPaddle.h"

struct RegenPaddle
{
    uint32_t (*get_paddle_position)(void);
    uint32_t lower_deadzone;
    uint32_t upper_deadzone;
    uint32_t raw_paddle_position;
    uint32_t mapped_paddle_position;
};

struct RegenPaddle *App_RegenPaddle_Create(
    uint32_t (*const get_pedal_position)(void),
    uint32_t lower_deadzone,
    uint32_t upper_deadzone)
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

ErrorCode App_RegenPaddle_Tick(struct RegenPaddle *const regen_paddle)
{
    const uint32_t paddle_position = regen_paddle->get_paddle_position();

    if (paddle_position > 100)
    {
        return ERROR_CODE_OUT_OF_RANGE;
    }

    regen_paddle->raw_paddle_position = paddle_position;

    if (regen_paddle->raw_paddle_position > regen_paddle->upper_deadzone)
    {
        regen_paddle->mapped_paddle_position = 100;
    }
    else if (regen_paddle->raw_paddle_position < regen_paddle->lower_deadzone)
    {
        regen_paddle->mapped_paddle_position = 0;
    }
    else
    {
        regen_paddle->mapped_paddle_position =
            100 *
            (regen_paddle->raw_paddle_position - regen_paddle->lower_deadzone) /
            (regen_paddle->upper_deadzone - regen_paddle->lower_deadzone);
    }

    return ERROR_CODE_OK;
}

uint32_t App_RegenPaddle_GetMappedPaddlePosition(
    const struct RegenPaddle *const regen_paddle)
{
    return regen_paddle->mapped_paddle_position;
}

uint32_t App_RegenPaddle_GetRawPaddlePosition(
    const struct RegenPaddle *const regen_paddle)
{
    return regen_paddle->raw_paddle_position;
}
