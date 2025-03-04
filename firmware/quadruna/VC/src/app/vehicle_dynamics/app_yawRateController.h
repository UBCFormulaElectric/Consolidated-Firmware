#pragma once

#include "app_pid.h"

/*
 * Yaw Rate Controller
 *
 * This system allows for effective torque vectoring by targeting the
 * proper yaw rates desired by the driver. This allows us to create positive
 * yaw moments on corner entry and negative yaw moments on exit.
 *
//  * Currently adapted for 2 wheel driving, main differences:
//  *
//  * lambda is originally intended to create a difference in torques between
//  * left and right wheels and then to transition this difference from front
//  * to back. For 2 wheel, it just provides a torque difference to the rear wheels
//  *
//  * Only calculating torques for the rear wheels
 */

// create static variables?

typedef struct
{
    PID *pid;
    // Tuning constants
    float ku; // Understeer Gradient: ku > 0 for understeerm ku < 0 for oversteer, ku = 0 for neutral steer
    float k4; // Torque distribution constants
    float k5; // Torque distribution constants
    // Inputs
    float requested_torque;
    float wheel_angle_deg;
    float vehicle_velocity_kmh;
    float real_yaw_rate_deg;
    // Outputs
    float ref_yaw_rate_deg;
    float yaw_moment;
    float torque_fl_Nm;
    float torque_fr_Nm;
    float torque_rl_Nm;
    float torque_rr_Nm;
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
void app_yawRateController_computeRefYawRate(YawRateController *yrc);

/*
 * Computes a corrective yaw moment using PID based on reference and real yaw rate
 */
void app_yawRateController_pidCompute(YawRateController *yrc);

/*
 * Computes the torques to send to the wheels based on the
 * corrective yaw moment and reference yaw rate
 */
void app_yawRateController_computeTorque(YawRateController *yrc);