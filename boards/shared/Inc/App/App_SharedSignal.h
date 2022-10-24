#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "configs/App_SharedSignalConfig.h"

#ifndef World
#error "Please define the 'World' type"
#endif

typedef enum {
    SIGNAL_ENTRY_HIGH, // Entry: Alert is now active.
    SIGNAL_EXIT_HIGH,  // Exit: Alert is not active.
} SignalState;

struct Signal;

/**
 * Allocate and initialize a signal
 * @param initial_time_ms The initial time, in milliseconds, used to initialize
 *                        the internal state of the signal
 * @param is_entry_condition_high A function that can be called to check if the
 * entry condition for the signal is high
 * @param is_exit_condition_high A function that can be called to check if the
 * exit condition for the signal is high
 * @param world The world associated with the entry and exit conditions for the
 * signal
 * @param callback The signal callback for the signal
 * @return The created signal, whose ownership is given to the caller
 */
struct Signal *App_SharedSignal_Create(
    uint32_t       entry_time,
    uint32_t       exit_time
);

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
SignalState App_SharedSignal_Update(struct Signal *signal, bool entry_condition_high, bool exit_condition_high);

// Getters
/**
 * Check if the callback function for the given signal is triggered
 * @param signal The signal to check if the callback function is triggered
 * @return true if the callback function is triggered, false if it is not
 */
bool App_SharedSignal_IsCallbackTriggered(const struct Signal *const signal);

/**
 * Deallocate the memory used by the given signal
 * @param signal The signal to deallocate
 */
void App_SharedSignal_Destroy(struct Signal *signal);
