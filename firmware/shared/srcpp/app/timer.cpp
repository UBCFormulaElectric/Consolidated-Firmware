#include "app/timer.hpp"
#include "io/time.hpp"

namespace app
{
void Timer::restart()
{
    start_time_ms = io::time::getCurrentMs();
    state         = TimerState::RUNNING;
}

void Timer::stop()
{
    state = TimerState::IDLE;
}

Timer::TimerState Timer::updateAndGetState()
{
    // If timer running and duration has elapsed, set it to expired
    // Otherwise, leave the timer alone
    if (state == TimerState::RUNNING && getElapsedTime() >= duration_ms)
        state = TimerState::EXPIRED;
    return state;
}

Timer::TimerState Timer::runIfCondition(bool condition)
{
    // If condition is false, stop the timer
    if (!condition)
        stop();
    // If timer idle and condition is set, start the timer
    else if (state == TimerState::IDLE) // condition is true
        restart();
    // Otherwise, just update the timer
    else
        updateAndGetState();

    return state;
}

uint32_t Timer::getElapsedTime() const
{
    uint32_t elapsed_time;
    switch (state)
    {
        case TimerState::RUNNING:
        {
            // Get elapsed time, but clamp to duration
            uint32_t total_elapsed_time = io::time::getCurrentMs() - start_time_ms;
            elapsed_time                = (total_elapsed_time > duration_ms) ? duration_ms : total_elapsed_time;
            break;
        }
        case TimerState::EXPIRED:
        {
            // Timer expired, so elapsed time is just the duration
            // It'd be confusing if elapsed time continued to run after expiry, since these timers are supposed to be
            // akin to stopwatches
            elapsed_time = duration_ms;
            break;
        }
        case TimerState::IDLE:
        default:
        {
            // Timer is stopped, assume no time elapsed
            elapsed_time = 0;
            break;
        }
    }
    return elapsed_time;
}
} // namespace app
