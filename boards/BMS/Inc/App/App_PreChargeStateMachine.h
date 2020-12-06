#pragma once

#include "App_PrechargeState.h"

struct PreChargeStateMachine;

/**
 *
 * @return
 */
struct PreChargeStateMachine *App_PreChargeStateMachine_Create(void);

/**
 *
 * @param world
 * @return
 */
struct PreChargeState *App_PreChargeStateMachine_GetCurrentState(
    struct PreChargeStateMachine *const state_machine);

/**
 *
 * @param state_machine
 * @param next_state
 */
void App_PreChargeStateMachine_SetNextState(
    struct PreChargeSequence *state_machine,
    struct PreChargeState *   next_state);

/**
 *
 * @param world
 */
void App_PreChargeStateMachine_Tick(struct BmsWorld *world);
