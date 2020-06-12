#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

/**
 * Check if the encoder alarm for the primary APPS in the given world is active
 * @param world The world containing the primary APPS to check for
 * @return true if the encoder alarm for the primary APPS in the given world is
 *         active, else false
 */
bool App_AcceleratorPedalSignals_IsPappsAlarmActive(struct FsmWorld *world);

/**
 * The callback function for the signal associated with the encoder alarm for
 * the primary APPS in the given world
 * @param world The world containing the primary APPS
 */
void App_AcceleratorPedalSignals_PappsAlarmCallback(struct FsmWorld *world);

/**
 * Check if the encoder alarm for the secondary APPS in the given world is
 * active
 * @param world The world containing the secondary APPS to check for
 * @return true if the encoder alarm for the secondary APPS in the given world
 * is active, else false
 */
bool App_AcceleratorPedalSignals_IsSappsAlarmActive(struct FsmWorld *world);

/**
 * The callback function for the signal associated with the encoder alarm for
 * the secondary APPS in the given world
 * @param world The world containing the secondary APPS
 */
void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world);
