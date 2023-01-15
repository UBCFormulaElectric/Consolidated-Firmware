#include "App_Timer.h"

static uint32_t current_time_ms;

void App_Timer_InitTimer(TimerChannel *const timer, uint32_t duration_ms)
{
    timer->duration_ms   = duration_ms;
    timer->state         = TIMER_STATE_IDLE;
    timer->start_time_ms = 0;
}

void App_Timer_Restart(TimerChannel *const timer)
{
    timer->start_time_ms = current_time_ms;
    timer->state         = TIMER_STATE_RUNNING;
}

void App_Timer_Stop(TimerChannel *const timer)
{
    timer->state = TIMER_STATE_IDLE;
}

TimerState App_Timer_UpdateAndGetState(TimerChannel *const timer)
{
    // If timer running and duration has elapsed, set it to expired
    // Otherwise, leave the timer alone
    if (timer->state == TIMER_STATE_RUNNING && App_Timer_GetElapsedTime(timer) >= timer->duration_ms)
    {
        timer->state = TIMER_STATE_EXPIRED;
    }

    return timer->state;
}

TimerState App_Timer_RunIfCondition(TimerChannel *const timer, bool condition)
{
    // If condition is false, stop the timer
    if (!condition)
    {
        App_Timer_Stop(timer);
    }
    // If timer idle and condition is set, start the timer
    else if (timer->state == TIMER_STATE_IDLE && condition)
    {
        App_Timer_Restart(timer);
    }
    // Otherwise, just update the timer
    else
    {
        App_Timer_UpdateAndGetState(timer);
    }

    return timer->state;
}

uint32_t App_Timer_GetElapsedTime(const TimerChannel *const timer)
{
    uint32_t elapsed_time;
    switch (timer->state)
    {
        case TIMER_STATE_RUNNING:
        {
            // Get elapsed time, but clamp to duration
            uint32_t total_elapsed_time = current_time_ms - timer->start_time_ms;
            elapsed_time = (total_elapsed_time > timer->duration_ms) ? timer->duration_ms : total_elapsed_time;
            break;
        }
        case TIMER_STATE_EXPIRED:
        {
            // Timer expired, so elapsed time is just the duration
            // It'd be confusing if elapsed time continued to run after expiry, since these timers are supposed to be
            // akin to stopwatches
            elapsed_time = timer->duration_ms;
            break;
        }
        case TIMER_STATE_IDLE:
        default:
        {
            // Timer is stopped, assume no time elapsed
            elapsed_time = 0;
            break;
        }
    }

    return elapsed_time;
}

void App_Timer_SetCurrentTimeMS(uint32_t time)
{
    current_time_ms = time;
}

uint32_t App_Timer_GetCurrentTimeMS()
{
    return current_time_ms;
}