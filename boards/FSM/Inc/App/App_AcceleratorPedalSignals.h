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
 * @param world The world containing the can_tx interface used to set can_tx
 * signals associated with the primary APPS alarm
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
 * @param world The world containing the can_tx interface used to set can_tx
 * signals associated with the secondary APPS alarm
 */
void App_AcceleratorPedalSignals_SappsAlarmCallback(struct FsmWorld *world);

/**
 * Check if the encoder alarm for the primary and secondary APPS in the given
 * world are inactive
 * @param world The world containing the primary and secondary APPS to check for
 * @return true if the encoder alarm for the secondary APPS in the given world
 *         is active, else false
 */
bool App_AcceleratorPedalSignals_IsPappsAndSappsAlarmInactive(struct FsmWorld *world);

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
 * @param world The world containing the can_tx interface used to set can_tx
 * signals associated with the APPS disagreement
 */
void App_AcceleratorPedalSignals_AppsDisagreementCallback(struct FsmWorld *world);
/**
 * Check if the mapped pedal percentage for the primary and secondary APPS in
 * the given world are in agreement
 * @param world The world containing the primary and secondary APPS to check for
 * @return true if the primary and secondary APPS are in agreement, else false
 */
bool App_AcceleratorPedalSignals_HasAppsAgreement(struct FsmWorld *world);

/**
 * Check if the APPS and Brake plausibility check failed
 * @note The APPS and brake plausibility check is considered to have failed when
 * the mapped pedal percentage is greater than 25% while the brake is
 * simultaneously actuated
 * @param world The world containing the brake and the primary APPS
 * @return true if the APPS and Brake plausibility check has failed, else false
 */
bool App_AcceleratorPedalSignals_HasAppsAndBrakePlausibilityFailure(struct FsmWorld *world);

/**
 * The callback function for the signal associated with the APPS and Brake
 * plausibility check failure for the primary APPS and brake in the given world
 * @param world The world containing the can_tx interface used to set can_tx
 * signals associated with the APPS and brake plausibility failure
 */
void App_AcceleratorPedalSignals_AppsAndBrakePlausibilityFailureCallback(struct FsmWorld *world);

/**
 * Check if the APPS and Brake plausibility check is OK
 * @note The APPS and brake plausibility check is considered OK when the mapped
 * pedal percentage is less than 5% regardless of the brake state
 * @param world The world containing the primary APPS
 * @return true if the APPS and brake plausibility check is OK, else false
 */
bool App_AcceleratorPedalSignals_IsAppsAndBrakePlausibilityOk(struct FsmWorld *world);
