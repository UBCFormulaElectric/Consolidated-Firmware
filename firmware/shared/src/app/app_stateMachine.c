#include "app_stateMachine.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const State *next_state;
static const State *current_state;

#ifdef __arm__
static StaticSemaphore_t state_tick_mutex_storage;
static SemaphoreHandle_t state_tick_mutex;
#elif __unix__ || __APPLE__
static pthread_mutex_t state_tick_mutex;
#elif _WIN32
static HANDLE state_tick_mutex;
#endif

/**
 * Run the given tick function over the given state machine if the tick function
 * is not null
 *
 * @param tick_function The tick function to run over the state machine
 */
static void runTickFunction(void (*tick_function)())
{
    if (tick_function != NULL)
    {
        tick_function();
    }
}

static void runTickStateTransition(void)
{
#ifdef __arm__
    xSemaphoreTake(state_tick_mutex, portMAX_DELAY);
#elif __unix__ || __APPLE__
    pthread_mutex_lock(&(state_tick_mutex));
#elif _WIN32
    WaitForSingleObject(state_tick_mutex, INFINITE);
#endif
    // Check if we should transition states
    if (next_state != current_state)
    {
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
    next_state = state;
    app_stateMachine_tickTransitionState();
}

void app_stateMachine_tick100Hz(void)
{
    runTickFunction(current_state->run_on_tick_100Hz);
}

void app_stateMachine_tickTransitionState(void)
{
    runTickStateTransition();
}

#ifdef TARGET_TEST
void app_stateMachine_setCurrentState(const State *const state)
{
    next_state = state;
    runTickStateTransition();
}
#endif