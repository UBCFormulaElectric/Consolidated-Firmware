#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "App_SharedClock.h"

struct Clock
{
    uint32_t current_time_ms;
};

struct Clock *App_SharedClock_Create(void)
{
    struct Clock *clock = malloc(sizeof(struct Clock));
    assert(clock != NULL);

    clock->current_time_ms = 0;

    return clock;
}

void App_SharedClock_Destroy(struct Clock *clock)
{
    free(clock);
}

void App_SharedClock_SetCurrentTimeInMilliseconds(
    struct Clock *clock,
    uint32_t      current_time_ms)
{
    clock->current_time_ms = current_time_ms;
}

uint32_t App_SharedClock_GetCurrentTimeInMilliseconds(const struct Clock *clock)
{
    return clock->current_time_ms;
}

uint32_t App_SharedClock_GetCurrentTimeInSeconds(const struct Clock *clock)
{
    // The time goes from 0 to 2^32 milliseconds or 0 to 2^32/1000 ≈
    // 4294967 seconds. Since we are using the floorf() function, we must
    // make sure floating point can represent all the integer values we need.
    //
    // The 32-bit floating point type can represent all positive integers
    // requiring 24 bits or less, or in other words, less than or equal to
    // 2^24 - 1 = 16777215. This is sufficient for representing the largest
    // number of seconds (i.e. 4924967 seconds) so using floorf() is safe.
    return floorf((float)clock->current_time_ms / 1000.0f);
}
