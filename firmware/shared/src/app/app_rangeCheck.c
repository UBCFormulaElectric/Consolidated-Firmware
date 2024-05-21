#include "app_rangeCheck.h"
#include "app_utils.h"

RangeCheckStatusMetaData app_rangeCheck_getValue(const RangeCheck *check, float value)
{
    RangeCheckStatus status;
    if (value < check->min_value)
        status = VALUE_UNDERFLOW;
    else if (value > check->max_value)
        status = VALUE_OVERFLOW;
    else
        status = VALUE_IN_RANGE;

    return (RangeCheckStatusMetaData){
        .status        = status,
        .clamped_value = CLAMP(value, check->min_value, check->max_value),
    };
}
