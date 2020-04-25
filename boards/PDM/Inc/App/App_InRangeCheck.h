#pragma once

struct InRangeCheck;

enum InRangeCheck_Status
{
    VALUE_IN_RANGE,
    VALUE_UNDERFLOW,
    VALUE_OVERFLOW,
};

/**
 * Allocate and initialize an in-range check to check whether a value is in
 * the given min/max value range
 * @get_voltage A function that can be called to get the value
 * @min_value Minimum value in the range, inclusive
 * @max_value Maximum value in the range, inclusive
 * @error_callback The function to call when the value is out of the specified
 *                 range
 * @return A pointer to the created in-range check, whose ownership is given
 * to the caller
 */
struct InRangeCheck *App_InRangeCheck_Create(
    float (*get_value)(void),
    float min_value,
    float max_value,
    void (*error_callback)(enum InRangeCheck_Status));

/**
 * Destroy the given in-range check
 * @param in_range_check The in-range check to destroy
 */
void App_InRangeCheck_Destroy(struct InRangeCheck *in_range_check);

/**
 * Get the value for the given in-range check
 * @param in_range_check The in-range check to get value for
 * @param value_buffer The buffer to store the value in
 * @return VALUE_IN_RANGE if the value was in the specified range
 *         VALUE_UNDERFLOW if the value was below the specified range
 *         VALUE_OVERFLOW if the value was above the specified range
 */
enum InRangeCheck_Status App_InRangeCheck_GetValue(
    const struct InRangeCheck *in_range_check,
    float *                    value_buffer);
