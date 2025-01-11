#pragma once

#include "app_vehicleDynamics.h"

#define MIN_SCALING_SPEED_kph 35.0f
#define SPEED_MIN_kph 5.0f
#define SOC_LIMIT_DERATING_VALUE 0.85f;
#define PEDAL_SCALE 0.3f
#define MAX_PEDAL_PERCENT 1.0f

/**
 * Initialize regen related attributes
 */
void app_regen_init(void);

/**
 * Runs when pedal percentage is in range [-100, 0] and does safety checks
 * before calculating and sending regenerative braking negative torque requests
 * @param accelerator_pedal_percentage is the pedal percentage
 */
void app_regen_run(float accelerator_pedal_percentage);

/**
 * Check if conditions allow for regenerative braking, if not reset
 * left and right torque struct to 0.0f
 * @param RegenBraking_Inputs struct provides for torque request
 * @param ActiveDifferential_Inputs struct provides for input into active differential algorithm
 * @return true or false depending on if it is safe to run regen
 */
bool app_regen_safetyCheck(struct RegenBraking_Inputs *regenAttr, ActiveDifferential_Inputs *inputs);

/**
 * Activate torque requests for regen
 * @param left is the left inverter torque request
 * @param right is the right inverter torque request
 */
void app_regen_sendTorqueRequest(float left, float right);

/**
 * Activate torque requests for regen
 * @param inputs are inputs for active differential
 * @param regenAttr struct provides for torque request
 * @param torqueRequest is the torque requst used in active
 * differential
 */
void app_regen_computeActiveDifferentialTorque(ActiveDifferential_Inputs *inputs, RegenBraking_Inputs *regenAttr);

/**
 * Remap Papps and Sapps pedal percentage
 * @param apps_pedal_percentage is the FSM given pedal percentage
 * @return remap pedal percentage from [0, 100] to [0.0, 1.0] to [-0.3, 0.7] and then scaled to [-1,1]
 */
float app_regen_pedalRemapping(float apps_pedal_percentage);