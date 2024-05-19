#include <stdlib.h>
#include <assert.h>

#include "app_rangeCheck.h"

RangeCheckStatusMetaData app_rangeCheck_getValue(const RangeCheck *check, float value)
{
    RangeCheckStatus status = VALUE_IN_RANGE;
    RangeCheckStatusMetaData return_value;

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

    return_value.value = value;
    return_value.status = status;

    return return_value;
}