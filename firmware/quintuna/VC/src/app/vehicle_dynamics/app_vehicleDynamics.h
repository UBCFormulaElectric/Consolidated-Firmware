#pragma once
#include "app_pid.h"
#include <stdbool.h>

typedef struct RegenBraking_Inputs
{
    bool  enable_active_differential;
    float battery_level;
    float left_inverter_torque_Nm;
    float right_inverter_torque_Nm;
    float derating_value;
} RegenBraking_Inputs;

typedef struct TractionControl_Inputs
{
    float torque_rr_Nm;
    float torque_rl_Nm;
    float torque_fr_Nm;
    float torque_fl_Nm;
    float motor_speed_rr_rpm;
    float motor_speed_rl_rpm;
    float motor_speed_fr_rpm;
    float motor_speed_fl_rpm;
    float vehicle_velocity_kmh;
    PID  *pid;
} TractionControl_Inputs;

typedef struct TractionControl_Outputs
{
    float torque_rr_final_Nm;
    float torque_rl_final_Nm;
    float torque_fr_final_Nm;
    float torque_fl_final_Nm;
} TractionControl_Outputs;

typedef struct ActiveDifferential_Inputs
{
    float wheel_angle_deg;
    float motor_speed_rr_rpm;
    float motor_speed_rl_rpm;
    float motor_speed_fr_rpm;
    float motor_speed_fl_rpm;
    float power_max_kW;
    float accelerator_pedal_percentage;
    float requested_torque_Nm;
} ActiveDifferential_Inputs;

typedef struct ActiveDifferential_Outputs
{
    float torque_rr_Nm;
    float torque_rl_Nm;
    float torque_fr_Nm;
    float torque_fl_Nm;
} ActiveDifferential_Outputs;

typedef struct PowerLimiting_Inputs
{
    float       left_motor_temp_C;
    float       right_motor_temp_C;
    const float current_based_power_limit_kW;
    const float regen_power_limit_kW;
    float       accelerator_pedal_percent;
} PowerLimiting_Inputs; // -- deprecated, leaving for regen 

typedef struct TorqueAllocationInputs // regardless of if controller is used or not final torques MUST go into this
                                      // struct before being sent to the inverters
{
    // float front_left_motor_torque;  uncomment for 4WD
    // float front_right_motor_torque;  uncomment for 4WD
    float power_limit_kw;
    float load_transfer_const;
    float total_torque_request;
    float rear_yaw_moment;
    float front_yaw_moment;
} TorqueAllocationInputs;

typedef struct TorqueAllocationOutputs
{
    float front_left_torque;
    float front_right_torque;
    float rear_left_torque;
    float rear_right_torque;
} TorqueAllocationOutputs;
