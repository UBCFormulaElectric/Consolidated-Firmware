#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Run the given tick function over the given state machine if the tick function
 * is not null
 *
 * @param state_machine The state machine to run the tick function over
 * @param tick_function The tick function to run over the state machine
 */
void app_stateMachine_tickIfNotNull(
    struct StateMachine *const state_machine,
    void (*tick_function)(struct StateMachine *))
{
    if (tick_function == NULL)
    {
        // Nothing to do
        return;
    }

#ifdef __arm__
    xSemaphoreTake(state_machine->state_tick_mutex, portMAX_DELAY);
#elif __unix__ || __APPLE__
    pthread_mutex_lock(&(state_machine->state_tick_mutex));
#elif _WIN32
    WaitForSingleObject(state_machine->state_tick_mutex, INFINITE);
#endif

    tick_function(state_machine);

    // Check if we should transition states
    if (state_machine->next_state != state_machine->current_state)
    {
        state_machine->current_state->run_on_exit(state_machine);
        state_machine->current_state = state_machine->next_state;
        state_machine->current_state->run_on_entry(state_machine);
    }

    // We assume the next time we tick we will continue in the current state,
    // unless told otherwise.
    state_machine->next_state = state_machine->current_state;

#ifdef __arm__
    xSemaphoreGive(state_machine->state_tick_mutex);
#elif __unix__ || __APPLE__
    pthread_mutex_unlock(&(state_machine->state_tick_mutex));
#elif _WIN32
    ReleaseMutex(state_machine->state_tick_mutex);
#endif
}

struct StateMachine *app_stateMachine_init(struct World *world, const struct State *initial_state)
{
    struct StateMachine *state_machine = (struct StateMachine *)malloc(sizeof(struct StateMachine));
    assert(state_machine != NULL);

    state_machine->world = world;

    state_machine->current_state = initial_state;
    state_machine->next_state    = initial_state;
    state_machine->current_state->run_on_entry(state_machine);
#ifdef __arm__
    state_machine->state_tick_mutex = xSemaphoreCreateMutexStatic(&(state_machine->state_tick_mutex_storage));
#elif __unix__ || __APPLE__
    pthread_mutex_init(&(state_machine->state_tick_mutex), NULL);
#elif _WIN32
    state_machine->state_tick_mutex = CreateMutex(NULL, FALSE, NULL);
#endif

    return state_machine;
}

const struct State *app_stateMachine_getCurrentState(const struct StateMachine *const state_machine)
{
    return state_machine->current_state;
}

void app_stateMachine_setNextState(struct StateMachine *const state_machine, const struct State *const next_state)
{
    state_machine->next_state = next_state;
}

void app_stateMachine_tick1Hz(struct StateMachine *const state_machine)
{
    app_stateMachine_tickIfNotNull(state_machine, state_machine->current_state->run_on_tick_1Hz);
}

void app_stateMachine_tick100Hz(StateMachine *const state_machine)
{
    app_stateMachine_tickIfNotNull(state_machine, state_machine->current_state->run_on_tick_100Hz);
}
