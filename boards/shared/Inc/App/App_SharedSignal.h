#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "configs/App_SharedSignalConfig.h"

#ifndef World
#error "Please define the 'World' type"
#endif

struct Signal;

struct SignalCallback
{
    // How long the signal's entry condition must be continuously high for, in
    // milliseconds, before the callback function is triggered
    uint32_t entry_high_duration_ms;

    // How long the signal's exit condition signal must be continuously high
    // for, in milliseconds, before the callback function is disabled
    uint32_t exit_high_duration_ms;

    // The callback function
    void (*function)(struct World *);
};

/**
 * Allocate and initialize a signal
 * @param initial_time_ms The initial time, in milliseconds, used to initialize
 *                        the internal state of the signal
 * @note If is_entry_high remains high for entry_high_ms the given callback
 * function will be triggered. The callback function will only stop triggering
 * if is_exit_high remains high for exit_high_ms.
 * @param is_entry_high A function that can be called to check if the
 * entry condition for the signal is high
 * @param is_exit_high A function that can be called to check if the
 * exit condition for the signal is high
 * @param callback The signal callback for the signal
 * @return The created signal, whose ownership is given to the caller
 */
struct Signal *App_SharedSignal_Create(
    uint32_t initial_time_ms,
    bool (*is_entry_high)(struct World *),
    bool (*is_exit_high)(struct World *),
    struct World *        world,
    struct SignalCallback callback);

/**
 * Deallocate the memory used by the given signal
 * @param signal The signal to deallocate
 */
void App_SharedSignal_Destroy(struct Signal *signal);

/**
 * Get the last time the given signal was observed to be low during entry, in
 * milliseconds
 * @param signal The signal to get the time during entry from
 * @return The last time the given signal was observed to be low during entry,
 * in milliseconds
 */
uint32_t App_SharedSignal_GetEntryLastTimeLowMs(const struct Signal *signal);

/**
 * Get the last time the given signal was observed to be high during entry, in
 * milliseconds
 * @param signal The signal to get the time during entry from
 * @return The last time the given signal was observed to be high during entry,
 * in milliseconds
 */
uint32_t App_SharedSignal_GetEntryLastTimeHighMs(const struct Signal *signal);

/**
 * Get the last time the given signal was observed to be low during exit, in
 * milliseconds
 * @param signal The signal to get the time during exit from
 * @return The last time the given signal was observed to be low during exit, in
 * milliseconds
 */
uint32_t App_SharedSignal_GetExitLastTimeLowMs(const struct Signal *signal);

/**
 * Get the last time the given signal was observed to be high during exit, in
 * milliseconds
 * @param signal The signal to get the time during exit from
 * @return The last time the given signal was observed to be high during exit,
 * in milliseconds
 */
uint32_t App_SharedSignal_GetExitLastTimeHighMs(const struct Signal *signal);

/**
 * Check if the callback function for the given signal is triggered
 * @param signal The signal to check if the callback function is triggered
 * @return true if the callback function is triggered, false if it is not
 */
bool App_SharedSignal_IsCallbackTriggered(const struct Signal *const signal);

/**
 * Update the internal state of the given signal. If the entry signal has been
 * continuously high for a period equal to or greater than the configured
 * duration (See: `entry_high_duration_ms) the callback function will be
 * triggered. The exit signal has to remain continuously high for a period equal
 * to or greater than the configured duration (See: `exit_signal_duration_ms) to
 * stop the callback function from triggering the exit signal.
 * @note This calls the is_entry_high() and is_exit_high() methods of the given
 * signal
 * @param signal The signal to update
 * @param current_time_ms The current time, in milliseconds
 */
void App_SharedSignal_Update(struct Signal *signal, uint32_t current_time_ms);
