#pragma once

typedef struct
{
    float (*get_value)(void);
    float min_value;
    float max_value;
} RangeCheck;

typedef enum
{
    VALUE_IN_RANGE,
    VALUE_UNDERFLOW,
    VALUE_OVERFLOW,
} RangeCheckStatus;

typedef struct
{
    RangeCheckStatus status;
    float value;
}RangeCheckStatusMetaData;

/**
 * Get the value for the given in-range check
 * @note Even if the value is out-of-range, this function still writes the value
 *       to the provided buffer. It is up the the caller to use the return value
 *       to determine whether the value in the buffer is in range.
 * @param in_range_check The in-range check to get value for
 * @param returned_value This will be set to the value of the given in-range
 *                       check
 * @return VALUE_IN_RANGE if the value is within the specified range
 *         VALUE_UNDERFLOW if the value is below the specified range
 *         VALUE_OVERFLOW if the value is above the specified range
 */
RangeCheckStatusMetaData app_rangeCheck_getValue(const RangeCheck *check, float value);