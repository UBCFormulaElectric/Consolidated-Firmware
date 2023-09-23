#include <assert.h>
#include <stdlib.h>
#include "App_SharedWaitSignal.h"

struct WaitSignal
{
    // The flag to indicate if the wait signal is currently waiting
    bool is_waiting;

    // The last time this wait signal was observed to be high, in milliseconds
    uint32_t last_time_high_ms;

    // The function to call to check if this wait signal is high
    bool (*is_high)(struct World *);

    // The world associated with this wait signal
    struct World *world;

    // The callback function triggered when the wait has completed
    struct WaitSignalCallback callback;
};

struct WaitSignal *App_SharedWaitSignal_Create(
    uint32_t initial_time_ms,
    bool (*is_high)(struct World *),
    struct World *const       world,
    struct WaitSignalCallback callback)
{
    struct WaitSignal *wait_signal = malloc(sizeof(struct WaitSignal));
    assert(wait_signal != NULL);

    wait_signal->is_waiting        = false;
    wait_signal->last_time_high_ms = initial_time_ms;
    wait_signal->is_high           = is_high;
    wait_signal->world             = world;
    wait_signal->callback          = callback;

    return wait_signal;
}

void App_SharedWaitSignal_Destroy(struct WaitSignal *wait_signal)
{
    free(wait_signal);
}

uint32_t App_SharedWaitSignal_GetLastTimeHighMs(const struct WaitSignal *const wait_signal)
{
    return wait_signal->last_time_high_ms;
}

bool App_SharedWaitSignal_IsWaiting(const struct WaitSignal *const wait_signal)
{
    return wait_signal->is_waiting;
}

void App_SharedWaitSignal_Update(struct WaitSignal *const wait_signal, uint32_t current_ms)
{
    if (!wait_signal->is_waiting)
    {
        // If the wait signal is high while a wait is not in progress, start
        // waiting
        if (wait_signal->is_high(wait_signal->world))
        {
            wait_signal->last_time_high_ms = current_ms;
            wait_signal->is_waiting        = true;
        }
    }
    else
    {
        uint32_t wait_duration_ms = current_ms - wait_signal->last_time_high_ms;

        if (wait_duration_ms >= wait_signal->callback.wait_duration_ms - 1U)
        {
            // Trigger the callback function once after the wait signal has
            // waited for the specified wait duration
            wait_signal->callback.function(wait_signal->world);
            wait_signal->is_waiting = false;
        }
    }
}
