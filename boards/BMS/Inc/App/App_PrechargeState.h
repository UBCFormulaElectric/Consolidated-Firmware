#pragma once

#include "App_BmsWorld.h"

#define MAX_NAME_LENGTH 16U

struct BmsWorld;
struct PreChargeState
{
    char name[MAX_NAME_LENGTH];
    void (*run_on_tick)(struct BmsWorld *);
};

/**
 *
 * @return
 */
struct PreChargeState *App_PreChargeState_GetInitState(void);

/**
 *
 * @return
 */
struct PreChargeState *App_PreChargeState_GetAIROpenState(void);

/**
 *
 * @return
 */
struct PreChargeState *App_PreChargeState_GetPrechargingState(void);

/**
 *
 * @param state
 * @param world
 */
void App_PreChargeState_RunOnTick(
    struct PreChargeState *state,
    struct BmsWorld *      world);
