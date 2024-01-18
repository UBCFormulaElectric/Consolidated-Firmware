#pragma once

#define MAX_STATE_NAME_LENGTH 16

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

#include "app_stateMachine.h"

typedef struct
{
    const struct State *next_state;
    const struct State *current_state;
#ifdef __arm__
    StaticSemaphore_t state_tick_mutex_storage;
    SemaphoreHandle_t state_tick_mutex;
#elif __unix__ || __APPLE__
    pthread_mutex_t state_tick_mutex;
#elif _WIN32
    HANDLE state_tick_mutex;
#endif
} StateMachine;

typedef struct
{
    // A newline terminated string containing the name of this state
    char name[MAX_STATE_NAME_LENGTH];

    void (*run_on_entry)(StateMachine *state_machine);
    void (*run_on_tick_1Hz)(StateMachine *state_machine);
    void (*run_on_tick_100Hz)(StateMachine *state_machine);
    void (*run_on_exit)(StateMachine *state_machine);
} State;

/**
 * Create a state machine with the given world
 * @param world A world that will be used by the state machine for all of it's
 *              interactions with the external world. The created state
 *              machine will *not* take ownership of this world, and it must
 *              be kept alive for the lifetime of the created state machine
 * @param initial_state The initial state to start the state machine in
 */
void app_stateMachine_init(StateMachine *state_machine, struct State *initial_state);

/**
 * Get the currently running state in the given state machine
 * @param state_machine The state machine to get the currently running state
 *                      from
 * @return The currently running state from the given state machine
 */
const struct State *app_stateMachine_getCurrentState(const struct StateMachine *state_machine);

/**
 * Set the next state the state machine should go to
 * @param state_machine The state machine to set the next state on
 * @param next_state The next state
 */
void app_stateMachine_setNextState(struct StateMachine *state_machine, const struct State *next_state);

/**
 * Get the world from the given state machine
 * @param state_machine The state machine to get the world from
 * @return The world from the given state machine
 */
struct World *App_SharedStateMachine_GetWorld(const struct StateMachine *state_machine);

/**
 * Tick the 1hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void app_stateMachine_tick1Hz(struct StateMachine *state_machine);

/**
 * Tick the 100Hz function of the given state machine
 * @param state_machine The state machine to tick
 */
void app_stateMachine_tick100Hz(struct StateMachine *state_machine);
