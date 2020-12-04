#pragma once

#include "App_BmsWorld.h"

struct BmsWorld;

/**
 *
 * @return
 */
struct PreChargeState *App_PreChargeState_GetInitState(void);

/**
 *
 * @return
 */
struct PreChargeState *App_PreChargeState_GetAirOpenState(void);

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
