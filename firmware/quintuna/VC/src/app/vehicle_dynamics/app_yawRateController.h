#pragma once

#include "app_pid.h"

/*
 * Yaw Rate Controller
 *
 * This system allows for effective torque vectoring by targeting the
 * proper yaw rates desired by the driver. This allows us to create positive
 * yaw moments on corner entry and negative yaw moments on exit.
 */

typedef struct
{
    PID *pid;
    /*
     * Understeer Gradient:
     *
     * ku > 0 for understeer
     * ku < 0 for oversteer,
     * ku = 0 for neutral steer
     */
    float ku;
    // Inputs
    float wheel_angle_rad;
    float vehicle_velocity_mps;
    float real_yaw_rate_rad;
} YawRateController;

typedef struct
{
    const PID_Config *pid_config;
    float             ku;
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
 * Runs the yaw rate controller, computing the reference yaw rate and the yaw moment
 */
void app_yawRateController_run(YawRateController *yrc);

/*
 * Get the reference yaw rate
 */
float app_yawRateController_getRefYawRateRad();

/*
 * Get the generated yaw moment
 */
float app_yawRateController_getYawMoment();