#include <stdlib.h>
#include <assert.h>

#include "app_rangeCheck.h"

RangeCheckStatus app_rangeCheck_getValue(const RangeCheck *check, float *returned_value)
{
    const float      value  = check->get_value();
    RangeCheckStatus status = VALUE_IN_RANGE;

    if (value < check->min_value)
    {
        status = VALUE_UNDERFLOW;
    }
    else if (value > check->max_value)
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
