#pragma once

#include "App_VehicleDynamicsControl.h"

#define MIN_SCALING_SPEED_kph 35.0f
#define SPEED_MIN_kph 5.0f

/**
 * Runs when pedal percentage is in range [-100, 0] and does safety checks
 * before calculating and sending regenerative braking negative torque requests
 * @param accelerator_pedal_percentage is the pedal percentage
 */
void App_Run_Regen(float accelerator_pedal_percentage);

/**
 * Check if conditions allow for regenerative braking, if not reset
 * left and right torque struct to 0.0f
 * @param RegenBraking_Inputs struct provides for torque request
 * @param ActiveDifferential_Inputs struct provides for input into active differential algorithm
 * @return true or false depending on if it is safe to run regen
 */
bool App_Regen_SafetyCheck(struct RegenBraking_Inputs * regenAttr, ActiveDifferential_Inputs * inputs);

/**
 * Activate torque requests for regen
 * @param left is the left inverter torque request
 * @param right is the right inverter torque request
 */
void App_Regen_Send_Torque_Request(float left, float right);

/**
 * Activate torque requests for regen
 * @param inputs are inputs for active differential
 * @param regenAttr struct provides for torque request
 * @param torqueRequest is the torque requst used in active
 * differential
 */
void App_ActiveDifferential_ComputeNegativeTorque(ActiveDifferential_Inputs * inputs, RegenBraking_Inputs * regenAttr);