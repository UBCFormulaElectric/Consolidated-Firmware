#pragma once
#include "torquevectoring/App_PID.h"

typedef struct PowerLimiting_Inputs
{
    float left_motor_temp_C;
    float right_motor_temp_C;
    float available_battery_power_kW;
    float accelerator_pedal_percent;
} PowerLimiting_Inputs;

typedef struct ActiveDifferential_Inputs
{
    float steering_angle_deg;
    int motor_speed_left_rpm;
    int motor_speed_right_rpm;
    float power_max_kW;
} ActiveDifferential_Inputs;

typedef struct ActiveDifferential_Outputs
{
    float torque_left_Nm;
    float torque_right_Nm;
} ActiveDifferential_Outputs;

typedef struct TractionControl_Inputs
{
    float torque_left_Nm;
    float torque_right_Nm;
    int motor_speed_left_rpm;
    int motor_speed_right_rpm;
    float front_wheel_speed_left_rpm;
    float front_wheel_speed_right_rpm;
    PID * pid;
} TractionControl_Inputs;

typedef struct TractionControl_Outputs
{
    float torque_left_final_Nm;
    float torque_right_final_Nm;
} TractionControl_Outputs;

void App_TorqueVectoring_Run(void);
