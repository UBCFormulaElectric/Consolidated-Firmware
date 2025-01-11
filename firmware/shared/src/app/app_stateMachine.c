#include "app_stateMachine.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifdef __arm__
#include <FreeRTOS.h>
#include <semphr.h>
#elif __unix__ || __APPLE__
#include <pthread.h>
#elif _WIN32
#include <windows.h>
#else
#error "Could not determine what CPU this is being compiled for."
#endif

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
void runTickFunction(void (*tick_function)())
{
#ifdef __arm__
    xSemaphoreTake(state_tick_mutex, portMAX_DELAY);
#elif __unix__ || __APPLE__
    pthread_mutex_lock(&(state_tick_mutex));
#elif _WIN32
    WaitForSingleObject(state_tick_mutex, INFINITE);
#endif

    if (tick_function != NULL)
    {
        tick_function();
    }

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

#ifdef __arm__
    xSemaphoreGive(state_tick_mutex);
#elif __unix__ || __APPLE__
    pthread_mutex_unlock(&(state_tick_mutex));
#elif _WIN32
    ReleaseMutex(state_tick_mutex);
#endif
}

void app_stateMachine_init(const State *initial_state)
{
    current_state = initial_state;
    next_state    = initial_state;

    if (current_state->run_on_entry != NULL)
    {
        current_state->run_on_entry();
    }

#ifdef __arm__
    state_tick_mutex = xSemaphoreCreateMutexStatic(&(state_tick_mutex_storage));
#elif __unix__ || __APPLE__
    pthread_mutex_init(&(state_tick_mutex), NULL);
#elif _WIN32
    state_tick_mutex = CreateMutex(NULL, FALSE, NULL);
#endif
}

const State *app_stateMachine_getCurrentState(void)
{
    return current_state;
}

void app_stateMachine_setNextState(const State *const state)
{
    next_state = state;
}

void app_stateMachine_tick1Hz(void)
{
    runTickFunction(current_state->run_on_tick_1Hz);
}

void app_stateMachine_tick100Hz(void)
{
    runTickFunction(current_state->run_on_tick_100Hz);
}
