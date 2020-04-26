#include <stdlib.h>
#include <assert.h>
#include "App_RegenPaddle.h"

struct RegenPaddle
{
    uint32_t (*get_raw_paddle_position)(void);
    uint32_t lower_deadzone;
    uint32_t upper_deadzone;
};

struct RegenPaddle *App_RegenPaddle_Create(
    uint32_t (*const get_raw_pedal_position)(void),
    uint32_t lower_deadzone,
    uint32_t upper_deadzone)
{
    struct RegenPaddle *regen_paddle = malloc(sizeof(struct RegenPaddle));

    assert(regen_paddle != NULL);

    regen_paddle->get_raw_paddle_position = get_raw_pedal_position;
    regen_paddle->lower_deadzone          = lower_deadzone;
    regen_paddle->upper_deadzone          = upper_deadzone;

    return regen_paddle;
}

void App_RegenPaddle_Destroy(struct RegenPaddle *const regen_paddle)
{
    free(regen_paddle);
}

ErrorCode App_RegenPaddle_GetRawPaddlePosition(
    const struct RegenPaddle *const regen_paddle,
    uint32_t *const                 returned_raw_paddle_position)
{
    const uint32_t raw_paddle_position =
        regen_paddle->get_raw_paddle_position();
    ErrorCode error_code = ERROR_CODE_OK;

    if (raw_paddle_position > 100)
    {
        error_code = ERROR_CODE_OUT_OF_RANGE;
    }
    else
    {
        *returned_raw_paddle_position = raw_paddle_position;
    }

    return error_code;
}

ErrorCode App_RegenPaddle_GetMappedPaddlePosition(
    const struct RegenPaddle *const regen_paddle,
    uint32_t *const                 returned_mapped_paddle_position)
{
    const uint32_t raw_paddle_position =
        regen_paddle->get_raw_paddle_position();
    ErrorCode error_code = ERROR_CODE_OK;

    if (raw_paddle_position > 100)
    {
        error_code = ERROR_CODE_OUT_OF_RANGE;
    }
    else
    {
        if (raw_paddle_position > regen_paddle->upper_deadzone)
        {
            *returned_mapped_paddle_position = 100;
        }
        else if (raw_paddle_position < regen_paddle->lower_deadzone)
        {
            *returned_mapped_paddle_position = 0;
        }
        else
        {
            *returned_mapped_paddle_position =
                100 * (raw_paddle_position - regen_paddle->lower_deadzone) /
                (regen_paddle->upper_deadzone - regen_paddle->lower_deadzone);
        }
    }

    return error_code;
}
