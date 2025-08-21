#pragma once

#include "app_vehicleDynamics.h"

#define MIN_SCALING_SPEED_kph 35.0f
#define SPEED_MIN_kph 5.0f
#define SOC_LIMIT_DERATING_VALUE 0.85f
#define PEDAL_SCALE 0.2f // 0.25f, 0.3f
#define MAX_PEDAL_PERCENT 1.0f

/**
 * Runs when pedal percentage is in range [-100, 0] and does safety checks
 * before calculating and sending regenerative braking negative torque requests
 * @param accelerator_pedal_percentage is the pedal percentage
 */
TorqueAllocationOutputs app_regen_run(float accelerator_pedal_percentage);

/**
 * Check if conditions allow for regenerative braking, if not reset
 * left and right torque struct to 0.0f
 * @param RegenBraking_Inputs struct provides for torque request
 * @param ActiveDifferential_Inputs struct provides for input into active differential algorithm
 * @return true or false depending on if it is safe to run regen
 */
bool app_regen_safetyCheck(struct RegenBraking_Inputs *regenAttr, ActiveDifferential_Inputs *inputs);

/**
 * Remap Papps and Sapps pedal percentage
 * @param apps_pedal_percentage is the FSM given pedal percentage
 * @return remap pedal percentage from [0.0, 1.0] to
 * regen range:        [0%-20%) [-0.2, 0.0) which then gets mapped [-1.0, 0.0)
 * cruising range:     [20%-30%) [0.0, 0.1) which then gets mapped to [0.0]
 * acceleration range: [30%-100%] [0.1, 0.8] which then gets mapped (0.0, 1.0]
 */
float app_regen_pedalRemapping(float apps_pedal_percentage);