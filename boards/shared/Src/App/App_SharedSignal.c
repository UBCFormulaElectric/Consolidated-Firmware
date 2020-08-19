#include <assert.h>
#include <stdlib.h>
#include "App_SharedSignal.h"

struct Signal
{
    bool is_callback_active;

    // The last time this signal was observed to be low, in milliseconds
    uint32_t last_time_low_ms;

    // The last time this signal was observed to be high, in milliseconds
    uint32_t last_time_high_ms;

    // The function to call to check if this signal entry condition is high
    bool (*is_entry_condition_high)(struct World *);

    // The function to call to check if the signal exit condition is high
    bool (*is_exit_condition_high)(struct World *);

    // The world associated with this signal
    struct World *world;

    // When and how to trigger the callback function for this signal
    struct SignalCallback callback;
};

struct Signal *App_SharedSignal_Create(
    uint32_t initial_time_ms,
    bool (*is_entry_condition_high)(struct World *),
    bool (*is_exit_condition_high)(struct World *),
    struct World *        world,
    struct SignalCallback callback)
{
    struct Signal *signal = malloc(sizeof(struct Signal));
    assert(signal != NULL);

    signal->is_callback_active      = false;
    signal->last_time_low_ms        = initial_time_ms;
    signal->last_time_high_ms       = initial_time_ms;
    signal->is_entry_condition_high = is_entry_condition_high;
    signal->is_exit_condition_high  = is_exit_condition_high;
    signal->world                   = world;
    signal->callback                = callback;

    return signal;
}

void App_SharedSignal_Destroy(struct Signal *signal)
{
    free(signal);
}

uint32_t App_SharedSignal_GetLastTimeLowMs(const struct Signal *signal)
{
    return signal->last_time_low_ms;
}

uint32_t App_SharedSignal_GetLastTimeHighMs(const struct Signal *signal)
{
    return signal->last_time_high_ms;
}

void App_SharedSignal_Update(struct Signal *signal, uint32_t current_time_ms)
{
    if (signal->is_callback_active)
    {
        if (signal->is_exit_condition_high(signal->world))
        {
            signal->last_time_high_ms = current_time_ms;
        }
        else
        {
            signal->last_time_low_ms   = current_time_ms;
            signal->is_callback_active = false;
        }

        const uint32_t time_since_last_low =
            current_time_ms - signal->last_time_low_ms;

        if (time_since_last_low >=
            signal->callback.exit_condition_high_duration_ms)
        {
            signal->is_callback_active = false;
        }
    }
    else
    {
        if (signal->is_entry_condition_high(signal->world))
        {
            signal->last_time_high_ms = current_time_ms;
        }
        else
        {
            signal->last_time_low_ms = current_time_ms;
        }

        const uint32_t time_since_last_low =
            current_time_ms - signal->last_time_low_ms;

        if (time_since_last_low >=
            signal->callback.entry_condition_high_duration_ms)
        {
            signal->callback.function(signal->world);
            signal->is_callback_active = true;
        }
    }
}
