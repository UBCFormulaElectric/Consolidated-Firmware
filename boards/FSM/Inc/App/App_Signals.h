#pragma once

#include "App_FsmWorld.h"

/**
 * Register signals for interfaces in the given world
 * @param world The world containing the interfaces to register signals for
 */
void App_Signals_Init(struct FsmWorld *world);
