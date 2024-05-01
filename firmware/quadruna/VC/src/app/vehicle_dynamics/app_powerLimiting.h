#pragma once
#include "app_vehicleDynamics.h"

/**
 * Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @note
 * @return A float (pointer?) for the maximum power allowed from the motor,
 */
float app_powerLimiting_computeMaxPower(struct PowerLimiting_Inputs *inputs);
