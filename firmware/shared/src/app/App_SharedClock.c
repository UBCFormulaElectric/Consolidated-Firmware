#include <assert.h>
#include <stdlib.h>
#include "App_SharedClock.h"

struct Clock
{
    uint32_t current_time_ms;
    uint32_t previous_time_ms;
};

struct Clock *App_SharedClock_Create(void)
{
    struct Clock *clock = malloc(sizeof(struct Clock));
    assert(clock != NULL);

    clock->current_time_ms  = 0U;
    clock->previous_time_ms = 0U;

    return clock;
}

void App_SharedClock_Destroy(struct Clock *clock)
{
    free(clock);
}

void App_SharedClock_SetCurrentTimeInMilliseconds(struct Clock *const clock, uint32_t current_time_ms)
{
    clock->current_time_ms = current_time_ms;
}

void App_SharedClock_SetPreviousTimeInMilliseconds(struct Clock *const clock, uint32_t previous_time_ms)
{
    clock->previous_time_ms = previous_time_ms;
}

uint32_t App_SharedClock_GetCurrentTimeInMilliseconds(const struct Clock *const clock)
{
    return clock->current_time_ms;
}

uint32_t App_SharedClock_GetCurrentTimeInSeconds(const struct Clock *const clock)
{
    return clock->current_time_ms / 1000U;
}

uint32_t App_SharedClock_GetPreviousTimeInMilliseconds(const struct Clock *const clock)
{
    return clock->previous_time_ms;
}

uint32_t App_SharedClock_GetPreviousTimeInSeconds(const struct Clock *const clock)
{
    return clock->previous_time_ms / 1000U;
}
