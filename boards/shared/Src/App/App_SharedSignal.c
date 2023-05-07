#include "App_SharedSignal.h"

#include <assert.h>
#include <stdlib.h>

#include "App_Timer.h"

struct Signal {
  // A flag used to indicate if the callback function is triggered
  bool is_signal_active;
  // The world associated with this signal
  struct World *world;

  // State of the signal
  SignalState state;

  TimerChannel entry_timer;
  TimerChannel exit_timer;
};

struct Signal *App_SharedSignal_Create(uint32_t entry_time,
                                       uint32_t exit_time) {
  struct Signal *signal = malloc(sizeof(struct Signal));
  assert(signal != NULL);

  signal->is_signal_active = false;
  signal->state = SIGNAL_STATE_CLEAR;

  App_Timer_InitTimer(&signal->entry_timer, entry_time);
  App_Timer_InitTimer(&signal->exit_timer, exit_time);

  return signal;
}

void App_SharedSignal_Destroy(struct Signal *signal) { free(signal); }

SignalState App_SharedSignal_Update(struct Signal *signal,
                                    bool entry_condition_high,
                                    bool exit_condition_high) {
  TimerState entry_timer_state =
      App_Timer_RunIfCondition(&signal->entry_timer, entry_condition_high);
  TimerState exit_timer_state =
      App_Timer_RunIfCondition(&signal->exit_timer, exit_condition_high);
  // either both false, or they are different
  assert((!entry_condition_high && !exit_condition_high) ||
         entry_condition_high != exit_condition_high);

  if (!signal->is_signal_active) {
    if (entry_timer_state == TIMER_STATE_EXPIRED) {
      signal->state = SIGNAL_STATE_ACTIVE;
      signal->is_signal_active = true;
    }
  } else {
    // keep running callback function for (exit_condition_high_duration_ms)
    // time.
    if (exit_timer_state == TIMER_STATE_EXPIRED) {
      signal->is_signal_active = false;
      signal->state = SIGNAL_STATE_CLEAR;
    }
  }
  return signal->state;
}

// Getters
bool App_SharedSignal_IsSignalActivated(const struct Signal *const signal) {
  return signal->is_signal_active;
}
