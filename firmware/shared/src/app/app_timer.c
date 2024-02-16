#include "app_timer.h"
#include "io_time.h"

void app_timer_init(TimerChannel *const timer, uint32_t duration_ms)
{
    timer->duration_ms   = duration_ms;
    timer->state         = TIMER_STATE_IDLE;
    timer->start_time_ms = 0;
}

void app_timer_restart(TimerChannel *const timer)
{
    timer->start_time_ms = io_time_getCurrentMs();
    timer->state         = TIMER_STATE_RUNNING;
}

void app_timer_stop(TimerChannel *const timer)
{
    timer->state = TIMER_STATE_IDLE;
}

TimerState app_timer_updateAndGetState(TimerChannel *const timer)
{
    // If timer running and duration has elapsed, set it to expired
    // Otherwise, leave the timer alone
    if (timer->state == TIMER_STATE_RUNNING && app_timer_getElapsedTime(timer) >= timer->duration_ms)
    {
        timer->state = TIMER_STATE_EXPIRED;
    }

    return timer->state;
}

TimerState app_timer_runIfCondition(TimerChannel *const timer, bool condition)
{
    // If condition is false, stop the timer
    if (!condition)
    {
        app_timer_stop(timer);
    }
    // If timer idle and condition is set, start the timer
    else if (timer->state == TIMER_STATE_IDLE && condition)
    {
        app_timer_restart(timer);
    }
    // Otherwise, just update the timer
    else
    {
        app_timer_updateAndGetState(timer);
    }

    return timer->state;
}

uint32_t app_timer_getElapsedTime(const TimerChannel *const timer)
{
    uint32_t elapsed_time;
    switch (timer->state)
    {
        case TIMER_STATE_RUNNING:
        {
            // Get elapsed time, but clamp to duration
            uint32_t total_elapsed_time = io_time_getCurrentMs() - timer->start_time_ms;
            elapsed_time = (total_elapsed_time > timer->duration_ms) ? timer->duration_ms : total_elapsed_time;
            break;e
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
