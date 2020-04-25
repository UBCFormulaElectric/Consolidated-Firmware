#pragma once

struct InRangeCheck;

enum InRangeCheck_Status
{
    VALUE_IN_RANGE,
    VALUE_UNDERFLOW,
    VALUE_OVERFLOW,
    UNINITIALIZED_VALUE
};

/**
 * Allocate and initialize an in-range check to check whether a value is in
 * the given min/max value range
 * @get_voltage A function that can be called to get the value
 * @min_value Minimum value in the range, inclusive
 * @max_value Maximum value in the range, inclusive
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

enum InRangeCheck_Status App_InRangeCheck_GetValue(
    struct InRangeCheck *const in_range_check,
    float *                    value_buffer);
