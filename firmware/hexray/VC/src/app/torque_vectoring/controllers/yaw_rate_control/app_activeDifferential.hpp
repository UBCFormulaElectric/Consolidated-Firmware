#pragma once
#include "app_vd_datatypes.hpp"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Electronic Differential Speed Control
     *
     * Computes wheel speed difference Δω (Eq. 13) from yaw rate and steering angle.
     * Gearbox and per-wheel split are left to the caller.
     *
     * @param omega_v_ref Actual vehicle angular velocity (yaw rate) from IMU (rad/s)
     * @param steering_wheel_angle_deg Steering wheel angle in degrees
     * @return controllerTorqueDeltas with wheel_delta = Δω (rad/s)
     */
    controllerTorqueDeltas app_activeDifferential_computeTorque(float omega_v_ref, float steering_wheel_angle_deg);

#ifdef __cplusplus
}
#endif
