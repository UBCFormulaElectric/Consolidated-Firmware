#pragma once

#include <stdbool.h>

struct FsmWorld;

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
 *         is active, else false
 */
bool App_AcceleratorPedalSignals_IsSappsAlarmActive(struct FsmWorld *world);

/**
 * The callback function for the signal associated with the encoder alarm for
 * the secondary APPS in the given world
 * @param world The world containing the secondary APPS
 */
void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world);

/**
 * Check if the mapped pedal percentage for primary and secondary APPS have a
 * disagreement of 10% or more
 * @param world The world containing the primary and secondary APPS to check for
 * @return true if the disagreement is equal to or greater than 10%, else false
 */
bool App_AcceleratorPedalSignals_HasDisagreement(struct FsmWorld *world);

/**
 * The callback function for the signal associated with the APPS disagreement
 * for the primary and secondary APPS in the given world
 * @param world The world containing the primary and secondary APPS
 */
void App_AcceleratorPedalSignals_HasDisagreementCallback(
    struct FsmWorld *world);
