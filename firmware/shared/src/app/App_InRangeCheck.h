#pragma once

#include "App_SharedExitCode.h"

struct InRangeCheck;

enum InRangeCheck_Status
{
    VALUE_IN_RANGE,
    VALUE_UNDERFLOW,
    VALUE_OVERFLOW,
};

/**
 * Allocate and initialize an in-range check to check whether a value is in
 * the given min/max range
 * @get_value A function that can be called to get the value
 * @min_value Minimum value in the range, inclusive
 * @max_value Maximum value in the range, inclusive
 * @return The created in-range check, whose ownership is given to the caller
 */
struct InRangeCheck *App_InRangeCheck_Create(float (*get_value)(void), float min_value, float max_value);

/**
 * Deallocate the memory used by the given in-range check
 * @param in_range_check The in-range check to deallocate
 */
void App_InRangeCheck_Destroy(struct InRangeCheck *in_range_check);

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
enum InRangeCheck_Status
    App_InRangeCheck_GetValue(const struct InRangeCheck *const in_range_check, float *returned_value);
