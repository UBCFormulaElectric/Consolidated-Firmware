#pragma once

#include <stdbool.h> // Simulink parses this header as C, where bool needs stdbool.h

#include "tv_debug.h" // debug bus types

#ifdef __cplusplus
extern "C"
{
#endif
    void estimate_matlab(
        double    yaw_rate,
        double    a_x,
        double    a_y,
        double    omegas[4],
        double    steer,
        double    gps_v_x,
        double    gps_v_y,
        bool      enable_gps,
        double   *est_v_x,
        double   *est_v_y,
        tv_debug *debug);

    /**
     * Matlab Wrapper for update
     */
    void update_matlab(
        double       v_x,
        double       v_y,
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
        double       kappas[4],
        double       torque_max[4],
        double       torque_min[4],
        tv_debug    *debug);

    /**
     * Matlab wrapper for kappa_update (motor speed setpoints + speed controller)
     * omegas are the plant's wheel speeds in rad/s (converted to motor speeds inside); a_x / a_y are the plant's body
     * accelerations in g (converted to m/s^2 inside); torque_request is in Nm;
     * speed_request is the motor speed setpoint in rad/s (for the Simulink power limiter)
     */
    void kappa_update_matlab(
        double       kappas[4],
        double       v_x,
        double       a_x,
        double       a_y,
        const double omegas[4],
        const double Fx[4],
        const double Fy[4],
        const double My[4],
        const double torque_max[4],
        const double torque_min[4],
        double       torque_request[4],
        double       speed_request[4],
        double       radial_deflection[4]);
#ifdef __cplusplus
}
#endif
