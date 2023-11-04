#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedMacros.h"

#define DEG_TO_RAD(degrees) ((degrees) * (float)M_PI / 180.0f)
#define WHEELBASE_mm 1550
#define TRACK_WIDTH_mm 1100
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3f
typedef struct RegenBraking
{
    bool  enable_active_differential;
    float current_battery_level;
    float left_inverter_torque_Nm;
    float right_inverter_torque_Nm;
    float wheel_speed_left_kph;
    float wheel_speed_right_kph;
    float prev_torque_request_Nm;
} RegenBraking;

typedef struct ActiveDifferential_Inputs
{
    float steering_angle_deg;
    float accelerator_pedal_percentage;
} ActiveDifferential_Inputs;

/**
 * Run safety checks before starting regenerative braking and execute
 * regen if satisfied
 * @param accelerator_pedal_percentage is the pedal percentage
 */
void App_Run_Regen(float *prev_torque_request, float accelerator_pedal_percentage);

/**
 * Check if conditions allow for regenerative braking, if not reset
 * left and right torque struct to 0.0f
 * @param regenAttr struct provides for torque request
 * @return true or false depending on if it is safe to run regen
 */
bool App_Regen_Safety(struct RegenBraking *regenAttr);

/**
 * Activate torque requests for regen
 * @param left is the left inverter torque request
 * @param right is the right inverter torque request
 */
void App_Regen_Activate(float left, float right);

/**
 * Activate torque requests for regen
 * @param inputs are inputs for active differential
 * @param regenAttr struct provides for torque request
 * @param torqueRequest is the torque requst used in active
 * differential
 */
void App_ActiveDifferential_ComputeNegativeTorque(
    ActiveDifferential_Inputs *inputs,
    RegenBraking *             regenAttr,
    float                      torqueRequest);

/**
 * Wheel angle to speed delta for active differential equation
 * @param wheel_angle_deg is the steering wheel angle
 * @return the delta value of the steering wheel angle
 */
float App_ActiveDifferential_WheelAngleToSpeedDelta(float wheel_angle_deg);