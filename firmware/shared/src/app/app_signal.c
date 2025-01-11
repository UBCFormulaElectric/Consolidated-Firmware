#include <assert.h>
#include <stdlib.h>
#include "app_signal.h"

void app_signal_init(Signal *signal, uint32_t entry_time, uint32_t exit_time)
{
    assert(signal != NULL);

    signal->is_signal_active = false;
    signal->state            = SIGNAL_STATE_CLEAR;

    app_timer_init(&signal->entry_timer, entry_time);
    app_timer_init(&signal->exit_timer, exit_time);
}

SignalState app_signal_getState(Signal *signal, bool entry_condition_high, bool exit_condition_high)
{
    TimerState entry_timer_state = app_timer_runIfCondition(&signal->entry_timer, entry_condition_high);
    TimerState exit_timer_state  = app_timer_runIfCondition(&signal->exit_timer, exit_condition_high);
    // either both false, or they are different
    assert((!entry_condition_high && !exit_condition_high) || entry_condition_high != exit_condition_high);

    if (!signal->is_signal_active)
    {
        if (entry_timer_state == TIMER_STATE_EXPIRED)
        {
            signal->state            = SIGNAL_STATE_ACTIVE;
            signal->is_signal_active = true;
        }
    }
    else
    {
        // keep running callback function for (exit_condition_high_duration_ms) time.
        if (exit_timer_state == TIMER_STATE_EXPIRED)
        {
            signal->is_signal_active = false;
            signal->state            = SIGNAL_STATE_CLEAR;
        }
    }

    return signal->state;
}
