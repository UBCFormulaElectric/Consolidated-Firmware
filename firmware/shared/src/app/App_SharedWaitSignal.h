#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "configs/App_SharedSignalConfig.h"

#ifndef World
#error "Please define the 'World' type"
#endif

struct WaitSignal;

struct WaitSignalCallback
{
    // Wait duration before the callback function is called
    uint32_t wait_duration_ms;

    // The callback function to call when the wait has completed
    void (*function)(struct World *);
};

/**
 * Allocate and initialize a wait signal
 * @param initial_time_ms The initial time, in milliseconds, used to initialize
 *                        the internal state of the signal
 * @param is_high A function that can be called to check if the wait signal is
 * high
 * @param world The world associated with the is_high function for the wait
 * signal
 * @param callback The signal callback for the wait signal
 * @return The created wait signal, whose ownership is given to the caller
 */
struct WaitSignal *App_SharedWaitSignal_Create(
    uint32_t initial_time_ms,
    bool (*is_high)(struct World *),
    struct World *            world,
    struct WaitSignalCallback callback);

/**
 * Deallocate the memory used by the given wait signal
 * @param wait_signal The wait signal to deallocate
 */
void App_SharedWaitSignal_Destroy(struct WaitSignal *wait_signal);

/**
 * Get the last time the given wait signal was observed to be high
 * @param wait_signal The wait signal to get the time from
 * @return The last time the given wait signal was observed to be low, in
 *         milliseconds
 */
uint32_t App_SharedWaitSignal_GetLastTimeHighMs(const struct WaitSignal *wait_signal);

/**
 * Check if the wait signal is waiting
 * @param wait_signal The given wait signal to check whether a wait is in
 * progress
 * @return true if the wait signal is waiting, false if it is not
 */
bool App_SharedWaitSignal_IsWaiting(const struct WaitSignal *wait_signal);

/**
 * Update the internal state of the given wait signal.
 * @note The callback function is triggered for one cycle after the signal has
 * waited for a given period of time (See: `wait_duration_ms`). To begin
 * waiting, the wait signal must remain high for at least one cycle. The signal
 * is not required to remain high through the entirety of the wait duration.
 * @param wait_signal The given wait signal to update
 * @param current_time_ms The current time, in milliseconds
 */
void App_SharedWaitSignal_Update(struct WaitSignal *wait_signal, uint32_t current_ms);
