#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Note: This driver does nothing to deal with integer overflow!
 * However, it'd take 50 days for that to happen, so nothing to worry about :)
 */

typedef enum
{
    TIMER_STATE_IDLE,    // Idle: Timer is stopped.
    TIMER_STATE_RUNNING, // Running: Timer is running, but its duration hasn't elapsed yet.
    TIMER_STATE_EXPIRED, // Expired: Timer ran, and has completed its duration.
} TimerState;

/**
 * TimerChannel struct is used to store all of a timer's data.
 * Note: Do not interact with member vars directly! Instead, use the functions defined later in this file.
 */
typedef struct
{
    uint32_t   duration_ms;   // Timer's duration, in milliseconds
    TimerState state;         // Current timer state
    uint32_t   start_time_ms; // Time that timer last started running, in ms
} TimerChannel;

/**
 * Initialize a timer, and set its duration.
 * @param timer The timer in question
 * @param duration_ms The duration of this timer, in milliseconds
 */
void App_Timer_InitTimer(TimerChannel* const timer, uint32_t duration_ms);

/**
 * Restart a timer, i.e. set the elapsed time back to 0. Leaves the timer in TIMER_STATE_RUNNING.
 * @param timer The timer in question
 */
void App_Timer_Restart(TimerChannel* const timer);

/**
 * Stop a timer. Leaves the timer in TIMER_STATE_IDLE.
 * @param timer The timer in question
 */
void App_Timer_Stop(TimerChannel* const timer);

/**
 * Update a timer and return the current state its in. Possible states are:
 * TIMER_STATE_IDLE: Timer is stopped.
 * TIMER_STATE_RUNNING: Timer is running, but its duration hasn't elapsed yet.
 * TIMER_STATE_EXPIRED: Timer ran, and has completed its duration. This is typically when your code will decide to do
 * something.
 * @param timer The timer in question
 * @return The updated state of the timer
 */
TimerState App_Timer_UpdateAndGetState(TimerChannel* const timer);

/**
 * If condition is true and timer isn't running, restart the timer. If condition is true and timer
 * is already running, allow the timer to continue running. If condition is false, stop the timer.
 * @param timer The timer in question
 * @param condition Whether or not to run timer
 * @return The updated state of the timer
 */
TimerState App_Timer_RunIfCondition(TimerChannel* const timer, bool condition);

/**
 * Get the elapsed time since timer started, in milliseconds. Note that if timer is TIMER_STATE_IDLE, the elapsed time
 * is zero, and if the timer is TIMER_STATE_EXPIRED, the elapsed time is the timer's duration.
 * @param timer The timer in question
 * @return Elapsed time
 */
uint32_t App_Timer_GetElapsedTime(const TimerChannel* const timer);

/**
 * Set the present time used by all timers. Add this to the 1kHz task, or timers won't work!
 * @param time Time to set
 */
void App_Timer_SetCurrentTimeMS(uint32_t time);

/**
 * Get the present time used by timers.
 * @return Current time
 */
uint32_t App_Timer_GetCurrentTimeMS(void);
