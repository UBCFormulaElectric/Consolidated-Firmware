#pragma once

#include <stdbool.h> // Simulink parses this header as C, where bool needs stdbool.h

#include "controls_debug.h" // debug bus types (same folder: Simulink only searches this folder)

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * Matlab wrapper for the full controller: estimator, update (high level controller + optimizer) and kappa_update
     * (motor speed setpoints + speed controller with the tire force feedforward + power limits)
     * All inputs are in the plant's SAE frame (y right); the accelerations are in g, apps and brake are 0..1, and the
     * wheel speeds are converted to motor speeds inside. v_x / v_y are not inputs: the estimator calculates them. The
     * slip targets, speed setpoints, tire feedforward and torque limits are in debug.
     */
    void update_matlab(
        double       yaw_rate_radps,
        double       a_x_g,
        double       a_y_g,
        double       apps,
        double       brake,
        double       steering_wheel_angle_rad,
        double       gps_v_x_mps,
        double       gps_v_y_mps,
        bool         enable_gps,
        const double wheel_omega_radps[4],
        double       discharge_limit_W,
        double       charge_limit_W,
        double       torque_request_Nm[4],
        tv_debug    *debug);
#ifdef __cplusplus
}
#endif
