#pragma once
#include "app_vehicleDynamics.h"
#include <stdbool.h>

/**
 * Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float (pointer?) for the maximum power allowed from the motor,
 * @return A float for the maximum power allowed from the motor,
 */
double app_powerLimiting_computeMaxPower(bool isRegenOn);

/**
 *
 * @param is_regen_mode
 * @param torqueToMotors
 * @param total_requestedPower * @param power_limit
 * @param derating_value
 */
void app_powerLimiting_torqueReduction(
    bool                     is_regen_mode,
    TorqueAllocationOutputs *torqueToMotors,
    double                   total_requestedPower,
    double                   power_limit,
    double                   derating_value);

/**
 * @param torques Torques allocated to the motors
 * @return Total power (kW) being requested by the motors
 */
double app_totalPower(const TorqueAllocationOutputs *torques);
