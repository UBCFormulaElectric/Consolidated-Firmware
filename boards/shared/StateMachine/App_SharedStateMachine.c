#include <stdbool.h>
#include <stddef.h>
#include "App_SharedStateMachine.h"
#include "SharedAssert.h"

static bool                shared_state_machine_initialized = false;
static const struct State *current_state;

static void App_TransitionState(const struct State *new_state)
{
    current_state = new_state;
    current_state->run_on_entry();
}

void App_SharedStateMachine_Init(struct State *initial_state)
{
    current_state                    = initial_state;
    shared_state_machine_initialized = true;
}

void App_SharedStateMachine_Tick(void)
{
    shared_assert(shared_state_machine_initialized == true);

    current_state->run_state_action();

    // Check if the state action caused a state transition
    const struct State* next_state = current_state->get_next_state();
    if (next_state != NULL)
    {
        current_state->set_next_state(NULL);
        current_state->run_on_exit();
        App_TransitionState(next_state);
    }
}
