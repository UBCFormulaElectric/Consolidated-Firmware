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
    void (*error_callback)(struct InRangeCheck *));

/**
 * Destroy the given in-range check
 * @param in_range_check The in-range check to destroy
 */
void App_InRangeCheck_Destroy(struct InRangeCheck *in_range_check);

/**
 * Update the status for the given in-range check
 * @param in_range_check The in-range check to update for
 */
void App_InRangeCheck_Tick(struct InRangeCheck *in_range_check);

/**
 * Get the status for the given in-range check
 * @param in_range_check The in-range check to get status for
 * @return The status for the given in-range check
 */
enum InRangeCheck_Status
    App_InRangeCheck_GetStatus(const struct InRangeCheck *in_range_check);

/**
 * Get the value for the given in-range check
 * @param in_range_check The in-range check to get value for
 * @return The value for the given in-range check
 */
float App_InRangeCheck_GetValue(const struct InRangeCheck *in_range_check);
