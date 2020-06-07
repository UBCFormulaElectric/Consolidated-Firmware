#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Signal;

/**
 *
 * @param is_high A function that can be called to check if the signal is high
 * @param initial_time_ms The initial time, in milliseconds, used to initialize
 *                        the internal state of the signal
 * @param duration_high_ms The duration, in milliseconds, that this signal must
 *                         be continuously high for before the callback function
 *                         is called
 * @param callback A callback function that is called when the signal has been
 *                 continuously high for a period equal to or greater than
 *                 `duration_high_ms`
 */
struct Signal *App_SharedSignal_Create(
    bool (*is_high)(void),
    uint32_t initial_time_ms,
    uint32_t duration_high_ms,
    void (*callback)(void));

/**
 * Deallocate the memory used by the given signal
 * @signal The signal to deallocate
 */
void App_SharedSignal_Destroy(struct Signal *signal);

/**
 * Get the last time the given signal was observed to be low, in milliseconds
 * @param signal The signal to get the time from
 * @return The last time the given signal was observed to be low, in
 *         milliseconds
 */
uint32_t App_SharedSignal_GetLastTimeLowMs(const struct Signal *signal);

/**
 * Get the last time the given signal was observed to be high, in milliseconds
 * @param signal The signal to get the time from
 * @return The last time the given signal was observed to be high, in
 *         milliseconds
 */
uint32_t App_SharedSignal_GetLastTimeHighMs(const struct Signal *signal);

/**
 * Update the internal state of the given signal. If the signal has been
 * continuously high for a period equal to or greater than the configured
 * duration (See: `duration_high_ms`), the callback function will be triggered.
 * @note This has the side-effect of calling the is_high() method of the given
 *       signal
 * @param signal The signal to update
 * @param current_time_ms The current time, in milliseconds
 */
void App_SharedSignal_Update(struct Signal *signal, uint32_t current_time_ms);
