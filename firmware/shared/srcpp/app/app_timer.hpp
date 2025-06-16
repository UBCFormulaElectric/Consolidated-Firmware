#pragma once

#include <cstdint>

namespace app
{

/**
 * Note: This driver does nothing to deal with integer overflow!
 * However, it'd take 50 days for that to happen, so nothing to worry about :)
 */
class Timer
{
  public:
    enum class TimerState
    {
        IDLE,    // Idle: Timer is stopped.
        RUNNING, // Running: Timer is running, but its duration hasn't elapsed yet.
        EXPIRED, // Expired: Timer ran, and has completed its duration.
    };

  private:
    uint32_t   duration_ms;   // Timer's duration, in milliseconds
    TimerState state;         // Current timer state
    uint32_t   start_time_ms; // Time that timer last started running, in ms

  public:
    /**
     * Initialize a timer, and set its duration.
     * @param timer The timer in question
     * @param duration_ms The duration of this timer, in milliseconds
     */
    explicit Timer(const uint32_t in_duration_ms)
      : duration_ms(in_duration_ms), state(TimerState::EXPIRED), start_time_ms(0){};

    /**
     * Restart a timer, i.e. set the elapsed time back to 0. Leaves the timer in TIMER_STATE_RUNNING.
     * @param timer The timer in question
     */
    void restart();

    /**
     * Stop a timer. Leaves the timer in TIMER_STATE_IDLE.
     * @param timer The timer in question
     */
    void stop();

    /**
     * Update a timer and return the current state its in. Possible states are:
     * TIMER_STATE_IDLE: Timer is stopped.
     * TIMER_STATE_RUNNING: Timer is running, but its duration hasn't elapsed yet.
     * TIMER_STATE_EXPIRED: Timer ran, and has completed its duration. This is typically when your code will decide to
     * do something.
     * @param timer The timer in question
     * @return The updated state of the timer
     */
    TimerState updateAndGetState();

    /**
     * If condition is true and timer isn't running, restart the timer. If condition is true and timer
     * is already running, allow the timer to continue running. If condition is false, stop the timer.
     * @param timer The timer in question
     * @param condition Whether or not to run timer
     * @return The updated state of the timer
     */
    TimerState runIfCondition(bool condition);

    /**
     * Get the elapsed time since timer started, in milliseconds. Note that if timer is TIMER_STATE_IDLE, the elapsed
     * time is zero, and if the timer is TIMER_STATE_EXPIRED, the elapsed time is the timer's duration.
     * @param timer The timer in question
     * @return Elapsed time
     */
    [[nodiscard]] uint32_t getElapsedTime() const;
};
} // namespace app
