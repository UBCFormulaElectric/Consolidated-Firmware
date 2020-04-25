#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"

struct InRangeCheck
{
    float (*get_value)(void);
    float min_value;
    float max_value;
    void (*error_callback)(enum InRangeCheck_Status);
};

struct InRangeCheck *App_InRangeCheck_Create(
    float (*const get_value)(void),
    float min_value,
    float max_value,
    void (*const error_callback)(enum InRangeCheck_Status))
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

enum InRangeCheck_Status App_InRangeCheck_GetValue(
    struct InRangeCheck *const in_range_check,
    float *const                    value_buffer)
{
    const float              value  = in_range_check->get_value();
    enum InRangeCheck_Status status = UNINITIALIZED_VALUE;

    if (value < in_range_check->min_value)
    {
        status = VALUE_UNDERFLOW;
        in_range_check->error_callback(status);
    }
    else if (value > in_range_check->max_value)
    {
        status = VALUE_OVERFLOW;
        in_range_check->error_callback(status);
    }
    else
    {
        status = VALUE_IN_RANGE;
    }

    *value_buffer = value;

    return status;
}
