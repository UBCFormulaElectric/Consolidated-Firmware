#pragma once

#include "App_ErrorCode.h"

struct InRangeCheck;

enum InRangeCheck_ErrorStatus
{
    VALUE_UNDERFLOW,
    VALUE_OVERFLOW,
};

/**
 * Allocate and initialize an in-range check to check whether a value is in
 * the given min/max range
 * @get_value A function that can be called to get the value
 * @min_value Minimum value in the range, inclusive
 * @max_value Maximum value in the range, inclusive
 * @error_callback The function to call when the value is outside of the
 *                 specified range. The argument is an error status that encodes
 *                 whether an underflow or overflow occured.
 * @return A pointer to the created in-range check, whose ownership is given
 * to the caller
 */
struct InRangeCheck *App_InRangeCheck_Create(
    float (*get_value)(void),
    float min_value,
    float max_value,
    void (*error_callback)(enum InRangeCheck_ErrorStatus));

void App_InRangeCheck_Destroy(struct InRangeCheck *in_range_check);

/**
 * Get the value for the given in-range check
 * @note Even if the value is out-of-range, this function still writes the value
 *       to the provided buffer. It is up the the caller to use the return value
 *       to determine whether the value in the buffer is in range.
 * @param in_range_check The in-range check to get value for
 * @param returned_value The buffer to store the value in
 * @return EXIT_CODE_OUT_OF_RANGE if the value is outside of the specified range
 */
ExitCode App_InRangeCheck_GetValue(
    const struct InRangeCheck *in_range_check,
    float *                    returned_value);
