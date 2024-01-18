#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "app_timer.h"

typedef enum
{
    SIGNAL_STATE_CLEAR,  // Exit: Alert is not active.
    SIGNAL_STATE_ACTIVE, // Entry: Alert is now active.
} SignalState;

typedef struct
{
    // A flag used to indicate if the callback function is triggered
    bool is_signal_active;
    // The world associated with this signal
    struct World *world;

    // State of the signal
    SignalState state;

    TimerChannel entry_timer;
    TimerChannel exit_timer;
} Signal;

/**
 * Initialize a signal
 * @param signal Signal to initialize
 * @param entry_time Amount of time required for the enter condition to be true to enter it
 * @param exit_time Amount of time required for the exit condition to be true to enter it
 * @return The created signal, whose ownership is given to the caller
 */
void app_signal_init(Signal *signal, uint32_t entry_time, uint32_t exit_time);

/**
 * Update the internal state of the given signal. If the entry condition for the
 * signal has been continuously high for a period equal to or greater than the
 * configured duration (See: `entry_high_duration_ms) the callback function will
 * be triggered. The exit signal for the signal has to remain continuously high
 * for a period equal to or greater than the configured duration (See:
 * `exit_signal_duration_ms) to stop triggering the callback function.
 * @note This calls the is_entry_condition_high() and is_exit_condition_high()
 * methods of the given signal
 * @param signal The signal to update
 * @param current_time_ms The current time, in milliseconds
 */
SignalState app_signal_getState(Signal *signal, bool entry_condition_high, bool exit_condition_high);
