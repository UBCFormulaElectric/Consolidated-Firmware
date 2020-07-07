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
 * Check if the mapped pedal percentage for the primary and secondary APPS in
 * the given world have a disagreement greater than 10%
 * @param world The world containing the primary and secondary APPS to check for
 * @return true if the disagreement is greater than 10%, else false
 */
bool App_AcceleratorPedalSignals_HasAppsDisagreement(struct FsmWorld *world);

/**
 * The callback function for the signal associated with the APPS disagreement
 * for the primary and secondary APPS in the given world
 * @param world The world containing the primary and secondary APPS
 */
void App_AcceleratorPedalSignals_AppsDisagreementCallback(
    struct FsmWorld *world);

/**
 * Check if the APPS and Brake plausibility check failed
 * @note The APPS and brake plausibility check is considered to have failed when
 * the mapped pedal percentage is greater than 25% while the brake is
 * simultaneously actuated
 * @param world The world containing the brake and the primary APPS
 */
bool App_AcceleratorPedalSignals_HasAppsAndBrakePlausibilityFailure(
    struct FsmWorld *world);

/**
 * The callback function for the signal associated with the APPS and Brake
 * plausibility check failure for the primary APPS and brake in the given world
 * @param world The world containing the primary APPS and the brake
 */
void App_AcceleratorPedalSignals_AppsAndBrakePlausibilityFailureCallback(
    struct FsmWorld *world);
