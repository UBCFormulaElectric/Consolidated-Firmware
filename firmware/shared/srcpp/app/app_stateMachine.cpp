#include "app_stateMachine.hpp"

#ifdef __arm__
#include <FreeRTOS.h>
#include <semphr.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <pthread.h>
#elif _WIN32
#include <windows.h>
#else
#error "Could not determine what CPU this is being compiled for."
#endif

#ifdef __arm__
static StaticSemaphore_t state_tick_mutex_storage;
static SemaphoreHandle_t state_tick_mutex;
#elif defined(__unix__) || defined(__APPLE__)
static pthread_mutex_t state_tick_mutex;
#elif _WIN32
static HANDLE state_tick_mutex;
#endif

namespace app
{
const State *state;
const State *next_state;

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
#elif defined(__unix__) || defined(__APPLE__)
    pthread_mutex_lock(&(state_tick_mutex));
#elif _WIN32
    WaitForSingleObject(state_tick_mutex, INFINITE);
#endif

    if (tick_function != NULL)
    {
        tick_function();
    }

    // Check if we should transition states
    if (next_state != state)
    {
        if (state->run_on_exit != NULL)
        {
            state->run_on_exit();
        }

        state = next_state;

        if (state->run_on_entry != NULL)
        {
            state->run_on_entry();
        }
    }

    // We assume the next time we tick we will continue in the current state,
    // unless told otherwise.
    next_state = state;

#ifdef __arm__
    xSemaphoreGive(state_tick_mutex);
#elif defined(__unix__) || defined(__APPLE__)
    pthread_mutex_unlock(&(state_tick_mutex));
#elif _WIN32
    ReleaseMutex(state_tick_mutex);
#endif
}

namespace StateMachine
{
    void init(const State *const initial_state)
    {
        state      = initial_state;
        next_state = initial_state;

        if (state->run_on_entry != NULL)
        {
            state->run_on_entry();
        }

#ifdef __arm__
        state_tick_mutex = xSemaphoreCreateMutexStatic(&(state_tick_mutex_storage));
#elif defined(__unix__) || defined(__APPLE__)
        pthread_mutex_init(&(state_tick_mutex), NULL);
#elif _WIN32
        state_tick_mutex = CreateMutex(NULL, FALSE, NULL);
#endif
    }

    void tick1Hz()
    {
        runTickFunction(state->run_on_tick_1Hz);
    }

    void tick100Hz()
    {
        runTickFunction(state->run_on_tick_100Hz);
    }

    const State *get_current_state()
    {
        return state;
    }

    void set_next_state(const State *const in_next_state)
    {
        next_state = in_next_state;
    }
} // namespace StateMachine
} // namespace app
