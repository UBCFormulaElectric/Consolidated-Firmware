#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"

struct InRangeCheck
{
    float (*get_value)(void);
    float                    min_value;
    float                    max_value;
    enum InRangeCheck_Status status;
    void (*error_callback)(struct InRangeCheck *);
};

struct InRangeCheck *App_InRangeCheck_Create(
    float (*const get_value)(void),
    float min_value,
    float max_value,
    void (*const error_callback)(struct InRangeCheck *))
{
    struct InRangeCheck *in_range_check = malloc(sizeof(struct InRangeCheck));

    assert(in_range_check != NULL);

    in_range_check->get_value      = get_value;
    in_range_check->min_value      = min_value;
    in_range_check->max_value      = max_value;
    in_range_check->status         = UNINITIALIZED_VALUE;
    in_range_check->error_callback = error_callback;

    return in_range_check;
}

void App_InRangeCheck_Destroy(struct InRangeCheck *const in_range_check)
{
    free(in_range_check);
}

void App_InRangeCheck_Tick(struct InRangeCheck *const in_range_check)
{
    const float value = in_range_check->get_value();

    if (value < in_range_check->min_value)
    {
        in_range_check->status = VALUE_UNDERFLOW;
        in_range_check->error_callback(in_range_check);
    }
    else if (value > in_range_check->max_value)
    {
        in_range_check->status = VALUE_OVERFLOW;
        in_range_check->error_callback(in_range_check);
    }
    else
    {
        in_range_check->status = VALUE_IN_RANGE;
    }
}

enum InRangeCheck_Status
    App_InRangeCheck_GetStatus(const struct InRangeCheck *const in_range_check)
{
    return in_range_check->status;
}

float App_InRangeCheck_GetValue(const struct InRangeCheck *const in_range_check)
{
    return in_range_check->get_value();
}
