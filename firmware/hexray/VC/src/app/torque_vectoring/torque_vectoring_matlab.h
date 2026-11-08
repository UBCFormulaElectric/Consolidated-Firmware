#pragma once

#include <stdbool.h> // Simulink parses this header as C, where bool needs stdbool.h

#include "tv_debug.h" // debug bus types

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * Matlab wrapper for the full controller: estimator, update (high level controller + optimizer) and kappa_update
     * (motor speed setpoints + speed controller with the tire force feedforward + power limits)
     * omegas are the plant's wheel speeds in rad/s (converted to motor speeds inside); discharge_pl / charge_pl are the
     * battery power limits in W; torque_request is in Nm. v_x / v_y are not inputs: the estimator calculates them. The
     * slip targets, speed setpoints, tire feedforward and torque limits are in debug.
     */
    void update_matlab(
        double       yaw_rate,
        double       a_x,
        double       a_y,
        double       apps,
        double       brake,
        double       steer,
        double       gps_v_x,
        double       gps_v_y,
        bool         enable_gps,
        const double omegas[4],
        double       discharge_pl,
        double       charge_pl,
        double       torque_request[4],
        tv_debug    *debug);
#ifdef __cplusplus
}
#endif
