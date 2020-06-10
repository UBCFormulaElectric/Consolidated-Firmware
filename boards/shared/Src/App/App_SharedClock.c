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
    return clock->current_time_ms / 1000U;
}
