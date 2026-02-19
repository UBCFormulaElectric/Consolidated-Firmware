#pragma once
#include "app_vehicleDynamics.h"

/**
 * Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @note
 * @return A float (pointer?) for the maximum power allowed from the motor,
 */
float app_powerLimiting_computeMaxPower(bool isRegenOn);

float getMaxMotorTemp(void);

void app_powerLimiting_torqueReduction(PowerLimitingInputs *inputs);

void app_power_limiting_TestRun_Torque_Reduction(PowerLimitingInputs *inputs);
app_pwer_limiting_TestRun_OverCurrent_Timer_Init(void);

float app_totalPower(TorqueAllocationOutputs *torques);
