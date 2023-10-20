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
    float wheel_angle_deg;
    float motor_speed_left_rpm;
    float motor_speed_right_rpm;
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
    float motor_speed_left_rpm;
    float motor_speed_right_rpm;
    float wheel_speed_front_left_kph;
    float wheel_speed_front_right_kph;
    PID * pid;
} TractionControl_Inputs;

typedef struct TractionControl_Outputs
{
    float torque_left_final_Nm;
    float torque_right_final_Nm;
} TractionControl_Outputs;

typedef struct Regen_Inputs
{
    float wheel_speed_front_left_kph;
    float wheel_speed_front_right_kph;
} Regen_Inputs;

typedef struct Regen_Outputs
{
    float regen_torque_left_Nm;
    float regen_torque_right_Nm;
} Regen_Outputs;

void App_TorqueVectoring_Setup(void);
void App_TorqueVectoring_Run(void);
void App_TorqueVectoring_HandleAcceleration(void);
