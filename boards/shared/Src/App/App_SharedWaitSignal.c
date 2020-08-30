#include <assert.h>
#include <stdlib.h>
#include "App_SharedWaitSignal.h"

struct WaitSignal
{
    bool     is_waiting;
    uint32_t last_ms;

    // If this function is high the timer starts ticking
    bool (*start_wait)(struct World *);

    // The world associated with this wait signal
    struct World *world;

    // The callback function triggered when the wait has completed
    struct WaitSignalCallback wait_signal_callback;
};

struct WaitSignal *App_SharedWaitSignal_Create(
    uint32_t initial_time_ms,
    bool (*start_wait)(struct World *),
    struct World *            world,
    struct WaitSignalCallback wait_signal_callback)
{
    struct WaitSignal *wait_signal = malloc(sizeof(struct WaitSignal));
    assert(wait_signal != NULL);

    wait_signal->is_waiting           = false;
    wait_signal->last_ms              = initial_time_ms;
    wait_signal->start_wait           = start_wait;
    wait_signal->world                = world;
    wait_signal->wait_signal_callback = wait_signal_callback;

    return wait_signal;
}

void App_SharedWaitSignal_Destroy(struct WaitSignal *wait_signal)
{
    free(wait_signal);
}

void App_SharedWaitSignal_Update(
    struct WaitSignal *wait_signal,
    uint32_t           current_ms)
{
    if (!wait_signal->is_waiting)
    {
        if (wait_signal->start_wait)
        {
            wait_signal->last_ms    = current_ms;
            wait_signal->is_waiting = true;
        }
    }
    else
    {
        uint32_t wait_duration_ms = current_ms - wait_signal->last_ms;

        if (wait_duration_ms >=
            wait_signal->wait_signal_callback.wait_duration_ms)
        {
            wait_signal->wait_signal_callback.wait_complete_callback(
                wait_signal->world);

            // Indicate that the signal is no longer waiting
            wait_signal->is_waiting = false;
        }
    }
}
