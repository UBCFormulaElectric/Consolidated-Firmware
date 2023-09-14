#pragma once
#include "torquevectoring/App_TorqueVectoring.h"

/**
 * Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @note
 * @return A float (pointer?) for the maximum power allowed from the motor,
 */
float App_PowerLimiting_ComputeMaxPower(struct PowerLimiting_Inputs *inputs);
