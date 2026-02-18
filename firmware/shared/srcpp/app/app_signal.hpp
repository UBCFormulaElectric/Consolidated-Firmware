// init with a consteval constructor
#pragma once
#include <cstdlib>
#include <cassert>
#include "app_timer.hpp"
/**
 * Initialize a signal object
 * @param entry_time Amount of time required for the enter condition to be true to enter it
 * @param exit_time Amount of time required for the exit condition to be true to enter it
 */

namespace app
{
class Signal
{
  public:
    // State of the signal
    enum class SignalState : uint8_t
    {
        CLEAR,  // Exit: Alert is not active.
        ACTIVE, // Entry: Alert is now active.
    };

  private:
    // A flag used to indicate if the callback function is triggered
    bool        is_signal_active = false;
    SignalState state;
    app::Timer  entry_timer;
    app::Timer  exit_timer;

  public:
    explicit Signal(uint32_t entry_time_ms, uint32_t exit_time_ms)
      : is_signal_active(false), state(SignalState::CLEAR), entry_timer(entry_time_ms), exit_timer(exit_time_ms)
    {
    }

    /**
     * Update the internal state of the given signal. If the entry condition for the
     * signal has been continuously high for a period equal to or greater than the
     * configured duration (See: `entry_high_ms) the callback function will
     * be triggered. The exit signal for the signal has to remain continuously high
     * for a period equal to or greater than the configured duration (See:
     * `exit_signal_ms) to stop triggering the callback function.
     * @param entry_condition_high
     * @param exit_condition_high
     */
    SignalState get_updated_state(bool entry_condition_high, bool exit_condition_high)
    {
        assert((!entry_condition_high && !exit_condition_high) || (entry_condition_high != exit_condition_high));

        const Timer::TimerState entry_timer_state = entry_timer.runIfCondition(entry_condition_high);
        const Timer::TimerState exit_timer_state  = exit_timer.runIfCondition(exit_condition_high);

        if (!is_signal_active)
        {
            if (entry_timer_state == Timer::TimerState::EXPIRED)
            {
                state            = SignalState::ACTIVE;
                is_signal_active = true;
            }
        }

        else
        {
            if (exit_timer_state == Timer::TimerState::EXPIRED)
            {
                is_signal_active = false;
                state            = SignalState::CLEAR;
            }
        }

        return state;
    }
    [[nodiscard]] constexpr bool        is_active() const { return is_signal_active; }
    [[nodiscard]] constexpr SignalState current_state() const { return state; }
};
} // namespace app