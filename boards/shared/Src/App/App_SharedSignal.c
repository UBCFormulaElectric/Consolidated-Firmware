#include <assert.h>
#include <stdlib.h>
#include "App_SharedSignal.h"
#include "App_Timer.h"

enum SignalState{
    SIGNAL_ENTRY_HIGH,  //Entry: Alert is now active.
    SIGNAL_EXIT_HIGH,   //Exit: Alert is not active.
};

struct Signal{
    // A flag used to indicate if the callback function is triggered
    bool is_callback_triggered;
    // The function to call to check if this signal's entry condition is high
    bool (*is_entry_condition_high)(struct World *);
    // The function to call to check if the signal's exit condition is high
    bool (*is_exit_condition_high)(struct World *);
    // The world associated with this signal
    struct World *world;

    // State of the signal
    enum SignalState state;

    TimerChannel * entry_timer;
    TimerChannel * exit_timer;
};

struct Signal *App_SharedSignal_Create(
    bool (*is_entry_condition_high)(struct World *),
    bool (*is_exit_condition_high)(struct World *),
    struct World * world,
    uint8_t entry_time,
    uint8_t exit_time
){
    struct Signal *signal = malloc(sizeof(struct Signal));
    assert(signal != NULL);

    signal->is_callback_triggered   = false;
    signal->is_entry_condition_high = is_entry_condition_high;
    signal->is_exit_condition_high  = is_exit_condition_high;
    signal->world                   = world;

    App_Timer_InitTimer(signal->entry_timer, entry_time);
    App_Timer_InitTimer(signal->exit_timer, exit_time);

    return signal;
}

void App_SharedSignal_Update(struct Signal *signal, uint32_t current_time_ms){
    TimerState entry_timer_state = App_Timer_RunIfCondition(signal->entry_timer, signal->is_entry_condition_high);
    TimerState exit_timer_state = App_Timer_RunIfCondition(signal->exit_timer, signal->is_exit_condition_high);

    if (!signal->is_callback_triggered){
        if (entry_timer_state == TIMER_STATE_EXPIRED){
            signal->state = SIGNAL_ENTRY_HIGH;
            signal->is_callback_triggered = true;
        }
    }
    else{
        //keep running callback function for (exit_condition_high_duration_ms) time.
        if (exit_timer_state == TIMER_STATE_EXPIRED)
            signal->is_callback_triggered = false;
        else
            signal->state = SIGNAL_EXIT_HIGH;
    }
}

//Getters
bool App_SharedSignal_IsCallbackTriggered(const struct Signal *const signal) { return signal->is_callback_triggered; }

//Destructor
void App_SharedSignal_Destroy(struct Signal *signal) { free(signal); }
