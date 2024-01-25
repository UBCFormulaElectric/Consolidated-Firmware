#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"

struct InRangeCheck
{
    float (*get_value)(void);
    float min_value;
    float max_value;
};

struct InRangeCheck *App_InRangeCheck_Create(float (*const get_value)(void), float min_value, float max_value)
{
    assert(get_value != NULL);
    assert(min_value <= max_value);

    struct InRangeCheck *in_range_check = malloc(sizeof(struct InRangeCheck));
    assert(in_range_check != NULL);

    in_range_check->get_value = get_value;
    in_range_check->min_value = min_value;
    in_range_check->max_value = max_value;

    return in_range_check;
}

void App_InRangeCheck_Destroy(struct InRangeCheck *const in_range_check)
{
    free(in_range_check);
}

enum InRangeCheck_Status
    App_InRangeCheck_GetValue(const struct InRangeCheck *const in_range_check, float *returned_value)
{
    const float              value  = in_range_check->get_value();
    enum InRangeCheck_Status status = VALUE_IN_RANGE;

    if (value < in_range_check->min_value)
    {
        status = VALUE_UNDERFLOW;
    }
    else if (value > in_range_check->max_value)
    {
        status = VALUE_OVERFLOW;
    }
    else
    {
        status = VALUE_IN_RANGE;
    }

    *returned_value = value;

    return status;
}
