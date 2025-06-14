#include "app_stateMachine.h"
#include "io_log.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const State *next_state;
static const State *current_state;

void app_stateMachine_init(const State *initial_state)
{
    current_state = initial_state;
    next_state    = initial_state;

    if (current_state->run_on_entry != NULL)
    {
        current_state->run_on_entry();
    }
}

const State *app_stateMachine_getCurrentState(void)
{
    return current_state;
}

void app_stateMachine_setNextState(const State *const state)
{
    next_state = state;
}

void app_stateMachine_tick100Hz(void)
{
    if (current_state->run_on_tick_100Hz != NULL)
    {
        current_state->run_on_tick_100Hz();
    }
}

void app_stateMachine_tickTransitionState(void)
{
    // Check if we should transition states
    if (next_state != current_state)
    {
        LOG_INFO("State transition: %s -> %s", current_state->name, next_state->name);

        if (current_state->run_on_exit != NULL)
        {
            current_state->run_on_exit();
        }

        current_state = next_state;

        if (current_state->run_on_entry != NULL)
        {
            current_state->run_on_entry();
        }
    }

    // We assume the next time we tick we will continue in the current state,
    // unless told otherwise.
    next_state = current_state;
}

#ifdef TARGET_TEST
void app_stateMachine_setCurrentState(const State *const state)
{
    if (current_state->run_on_exit != NULL)
    {
        current_state->run_on_exit();
    }

    current_state = state;

    if (current_state->run_on_entry != NULL)
    {
        current_state->run_on_entry();
    }

    next_state = state;
}
#endif
