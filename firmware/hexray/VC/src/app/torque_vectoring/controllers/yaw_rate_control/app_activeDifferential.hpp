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
     * Computes torque delta suggestions for each wheel based on vehicle yaw rate and steering angle,
     * implementing the algorithm from the electronic differential paper.
     *
     * @param omega_v_ref Actual vehicle angular velocity (yaw rate) from IMU (rad/s)
     * @param steering_wheel_angle_deg Steering wheel angle in degrees
     * @param omega_m_max Maximum motor speed (rad/s) for saturation. If <= 0, no saturation applied.
     * @return Torque deltas suggested by this controller (stored in controllerTorqueDeltas structure)
     */
    controllerTorqueDeltas
        app_activeDifferential_computeTorque(float omega_v_ref, float steering_wheel_angle_deg, float omega_m_max);

#ifdef __cplusplus
}
#endif
