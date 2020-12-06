#pragma once

#include <stdbool.h>
#include "App_BmsWorld.h"

struct BmsWorld;

/**
 *
 * @param world
 * @return
 */
bool App_PrechargeSignals_IsWaitingAfterInit(struct BmsWorld *world);

/**
 *
 * @param world
 */
void App_PrechargeSignals_WaitingAfterInitCompleteCallback(
    struct BmsWorld *world);
