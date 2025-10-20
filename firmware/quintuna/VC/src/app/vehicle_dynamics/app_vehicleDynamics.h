#pragma once

typedef struct TorqueAllocationOutputs
{
    double torque_fl;
    double torque_fr;
    double torque_rl;
    double torque_rr;
} TorqueAllocationOutputs;

// typedef struct RegenBraking_Inputs
// {
//     bool  enable_active_differential;
//     float battery_level;
//     float torque_rr_Nm;
//     float torque_rl_Nm;
//     float torque_fr_Nm;
//     float torque_fl_Nm;
//     float derating_value;
// } RegenBraking_Inputs;
//
// typedef struct TractionControl_Inputs
// {
//     float torque_rr_Nm;
//     float torque_rl_Nm;
//     float torque_fr_Nm;
//     float torque_fl_Nm;
//     float motor_speed_rr_rpm;
//     float motor_speed_rl_rpm;
//     float motor_speed_fr_rpm;
//     float motor_speed_fl_rpm;
//     float vehicle_velocity_kmh;
//     PID  *pid;
// } TractionControl_Inputs;
//
// typedef struct TractionControl_Outputs
// {
//     float torque_rr_final_Nm;
//     float torque_rl_final_Nm;
//     float torque_fr_final_Nm;
//     float torque_fl_final_Nm;
// } TractionControl_Outputs;
//
// typedef struct ActiveDifferential_Inputs
// {
//     float wheel_angle_deg;
//     float motor_speed_rr_rpm;
//     float motor_speed_rl_rpm;
//     float motor_speed_fr_rpm;
//     float motor_speed_fl_rpm;
//     float power_max_kW;
//     float accelerator_pedal_percentage;
//     float requested_torque_Nm;
//     float derating_value;
//     bool  is_regen_mode;
// } ActiveDifferential_Inputs;
//
// typedef struct ActiveDifferential_Outputs
// {
//     float torque_rr_Nm;
//     float torque_rl_Nm;
//     float torque_fr_Nm;
//     float torque_fl_Nm;
// } ActiveDifferential_Outputs;
//
// typedef struct TorqueAllocationInputs // regardless of if controller is used or not final torques MUST go into this
//                                       // struct before being sent to the inverters
// {

// } TorqueAllocationInputs;