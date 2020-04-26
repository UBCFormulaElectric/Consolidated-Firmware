#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"

struct InRangeCheck
{
    float (*get_value)(void);
    float min_value;
    float max_value;
    void (*error_callback)(enum InRangeCheck_ErrorStatus);
};

struct InRangeCheck *App_InRangeCheck_Create(
    float (*const get_value)(void),
    float min_value,
    float max_value,
    void (*const error_callback)(enum InRangeCheck_ErrorStatus))
{
    struct InRangeCheck *in_range_check = malloc(sizeof(struct InRangeCheck));

    assert(in_range_check != NULL);

    in_range_check->get_value      = get_value;
    in_range_check->min_value      = min_value;
    in_range_check->max_value      = max_value;
    in_range_check->error_callback = error_callback;

    return in_range_check;
}

void App_InRangeCheck_Destroy(struct InRangeCheck *const in_range_check)
{
    free(in_range_check);
}

ExitCode App_InRangeCheck_GetValue(
    const struct InRangeCheck *const in_range_check,
    float *const                     returned_value)
{
    const float value     = in_range_check->get_value();
    ExitCode    exit_code = EXIT_CODE_OUT_OF_RANGE;

    if (value < in_range_check->min_value)
    {
        in_range_check->error_callback(VALUE_UNDERFLOW);
    }
    else if (value > in_range_check->max_value)
    {
        in_range_check->error_callback(VALUE_OVERFLOW);
    }
    else
    {
        exit_code = EXIT_CODE_OK;
    }

    *returned_value = value;

    return exit_code;
}
