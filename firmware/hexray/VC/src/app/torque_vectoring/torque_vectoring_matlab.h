#pragma once

extern "C"
{
    /**
     * Matlab Wrapper for update
     */
    void update_matlab(
        double v_x,
        double v_y,
        double yaw_rate,
        double a_x,
        double a_y,
        double apps,
        double delta_fl,
        double delta_fr,
        double kappas[4],
        double torque_max[4],
        double torque_min[4]);

    /**
     * Matlab wrapper for kappa_update
     */
    void kappa_update_matlab(double kappas[4], double v_x, double oemgas[4]);

    /**
     * Matlab wrapper for estimate
     */
    void estimate_matlab(
        const double apps,
        const double steering_angle,
        const double a_x,
        const double a_y,
        const double yaw_rate,
        double       rpm[4],
        double       v_body_mps[2],
        double       delta[4]);
}
