#pragma once

#include "app_pid.h"
// typedef struct{
//     float steering_angle;
//     float vehicle_velocity;
//     float ku;
//     float k4;
//     float k5;
// } YawRateController_Inputs;

// typedef struct{
// float torque_left_Nm;
//     float torque_right_Nm;
// } YawRateController_Outputs

typedef struct
{
    PID  *pid;
    float requested_torque;
    float wheel_angle_deg;
    float vehicle_velocity;
    float real_yaw_rate;
    float ku;
    float k4;
    float k5;
    float torque_left_Nm;
    float torque_right_Nm;
} YawRateController;

typedef struct
{
    const PID_Config *pid_config;
    float       ku;
    float       k4;
    float       k5;
} YawRateController_Config;

void app_yawRateController_init(YawRateController *yrc, PID *pid, const YawRateController_Config *config);

float app_yawRateController_computeRefYawRate(YawRateController *yrc);

float app_yawRateController_pid_compute(YawRateController *yrc, float ref_yaw_rate);

void app_yawRateController_computeTorque(YawRateController *yrc, float ref_yaw_rate, float yaw_moment);