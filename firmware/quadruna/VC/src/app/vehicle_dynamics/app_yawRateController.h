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
    float             ku;
    float             k4;
    float             k5;
} YawRateController_Config;

/*
 * Initializes Yaw Rate Controller
 */
void app_yawRateController_init(YawRateController *yrc, PID *pid, const YawRateController_Config *config);

/*
 * Computes the reference yaw rate based on steering angle and velocity
 */
float app_yawRateController_computeRefYawRate(YawRateController *yrc);

/*
 * Computes a corrective yaw moment using PID based on reference and real yaw rate
 */
float app_yawRateController_pid_compute(YawRateController *yrc, float ref_yaw_rate);

/*
 * Computes the torques to send to the wheels based on the
 * corrective yaw moment and reference yaw rate
 */
void app_yawRateController_computeTorque(YawRateController *yrc, float ref_yaw_rate, float yaw_moment);