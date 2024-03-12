#pragma once
#include "app_pid.h"
#include <stdbool.h>

typedef struct RegenBraking_Inputs
{
    bool  enable_active_differential;
    float current_battery_level;
    float left_inverter_torque_Nm;
    float right_inverter_torque_Nm;
    float derating_value;
} RegenBraking_Inputs;

typedef struct TractionControl_Inputs
{
    float torque_left_Nm;
    float torque_right_Nm;
    float motor_speed_left_rpm;
    float motor_speed_right_rpm;
    float wheel_speed_front_left_kph;
    float wheel_speed_front_right_kph;
    PID  *pid;
} TractionControl_Inputs;

typedef struct TractionControl_Outputs
{
    float torque_left_final_Nm;
    float torque_right_final_Nm;
} TractionControl_Outputs;

typedef struct ActiveDifferential_Inputs
{
    float wheel_angle_deg;
    float motor_speed_left_rpm;
    float motor_speed_right_rpm;
    float power_max_kW;
    float accelerator_pedal_percentage;
} ActiveDifferential_Inputs;

typedef struct ActiveDifferential_Outputs
{
    float torque_left_Nm;
    float torque_right_Nm;
} ActiveDifferential_Outputs;

typedef struct PowerLimiting_Inputs
{
    float       left_motor_temp_C;
    float       right_motor_temp_C;
    const float power_limit_kW;
    float       accelerator_pedal_percent;
} PowerLimiting_Inputs;
