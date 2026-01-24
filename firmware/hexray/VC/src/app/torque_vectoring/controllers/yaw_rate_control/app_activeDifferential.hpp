#pragma once
#include "app_vehicleDynamics.hpp"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Electronic Differential Speed Control
     *
     * Computes motor speed references for left and right motors based on vehicle base speed
     * and steering angle, implementing the algorithm from the electronic differential paper.
     *
     * @param omega_v_ref Desired vehicle base angular speed (rad/s)
     * @param steering_wheel_angle_deg Steering wheel angle in degrees
     * @param omega_m_max Maximum motor speed (rad/s) for saturation. If <= 0, no saturation applied.
     * @return Motor speed references in rad/s (stored in TorqueAllocationOutputs structure)
     */
    TorqueAllocationOutputs
        app_activeDifferential_computeTorque(float omega_v_ref, float steering_wheel_angle_deg, float omega_m_max);

#ifdef __cplusplus
}
#endif
