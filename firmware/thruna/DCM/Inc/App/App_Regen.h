#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedMacros.h"

#define WHEELBASE_mm 1550
#define TRACK_WIDTH_mm 1100
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3f
#define MAX_TORQUE_CHANGE 6.0f
#define MIN_SCALING_SPEED_kph 35.0f
#define SPEED_MIN_kph 5.0f
#define MAX_PEDAL_POSITION -100.0f
#define MAX_REGEN_nm -30.0f
#define MAX_BATTERY_TEMP 45
#define SMALL_EPSILON .000001f
#define POWER_LIMIT_REGEN_kW 10.0f             // 10.5kW ~ 35A tbd varying limits?
#define POWER_TO_TORQUE_CONVERSION_FACTOR 9550 // as specified in the manual for the emrax 188 motors
#define MOTOR_TEMP_CUTOFF_c 90.0f
#define MOTOR_TEMP_POWER_DECREMENTING_RATIO \
    80.0f / 30.0f // amount to decrease max power by for every degree celsius over motor cutoff temp (we got this number
                  // by looking at the emrax 188 manual)

typedef struct RegenBraking
{
    bool  enable_active_differential;
    float current_battery_level;
    float left_inverter_torque_Nm;
    float right_inverter_torque_Nm;
    float motor_left_speed_rpm;
    float motor_right_speed_rpm;
} RegenBraking;

typedef struct ActiveDifferential_Inputs
{
    float steering_angle_deg;
    float accelerator_pedal_percentage;
    float power_max_kW;
} ActiveDifferential_Inputs;

typedef struct PowerLimiting_Inputs
{
    float left_motor_temp_C;
    float right_motor_temp_C;
    float available_battery_power_kW;
    float max_accelerator;
} PowerLimiting_Inputs;

/**
 * Runs when pedal percentage is in range [-100, 0] and does safety checks
 * before calculating and sending regenerative braking negative torque requests
 * @param accelerator_pedal_percentage is the pedal percentage
 */
void App_Run_Regen(float accelerator_pedal_percentage);

/**
 * Check if conditions allow for regenerative braking, if not reset
 * left and right torque struct to 0.0f
 * @param regenAttr struct provides for torque request
 * @return true or false depending on if it is safe to run regen
 */
bool App_Regen_SafetyCheck(struct RegenBraking *regenAttr);

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
void App_ActiveDifferential_ComputeNegativeTorque(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr);

/**
 * Wheel angle to speed delta for active differential equation
 * @param wheel_angle_deg is the steering wheel angle
 * @return the delta value of the steering wheel angle
 */
float App_ActiveDifferential_WheelAngleToSpeedDelta(float wheel_angle_deg);

float App_ActiveDifferential_PowerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr);