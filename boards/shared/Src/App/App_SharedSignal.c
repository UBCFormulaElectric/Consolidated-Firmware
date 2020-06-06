#include <assert.h>
#include <stdlib.h>
#include "App_SharedSignal.h"

struct Signal
{
    // The last time this signal was low, in milliseconds
    uint32_t last_time_low_ms;

    // The last time this signal was high, in milliseconds
    uint32_t last_time_high_ms;

    // The function to call to check if this signal is high
    bool (*is_high)(void);

    // How long this signal must be continuously high for, in milliseconds,
    // before the callback function is triggered
    uint32_t duration_high_ms;

    // The callback function for this signal
    void (*callback)(void);
};

struct Signal *App_SharedSignal_Create(
    bool (*is_high)(void),
    uint32_t initial_time_ms,
    uint32_t duration_high_ms,
    void (*callback)(void))
{
    struct Signal *signal = malloc(sizeof(struct Signal));
    assert(signal != NULL);

    signal->last_time_low_ms  = initial_time_ms;
    signal->last_time_high_ms = initial_time_ms;
    signal->is_high           = is_high;
    signal->duration_high_ms  = duration_high_ms;
    signal->callback          = callback;

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
    if (signal->is_high())
    {
        signal->last_time_high_ms = current_time_ms;
    }
    else
    {
        signal->last_time_low_ms = current_time_ms;
    }

    const uint32_t time_since_last_low =
        current_time_ms - signal->last_time_low_ms;

    if (time_since_last_low >= signal->duration_high_ms)
    {
        signal->callback();
    }
}
