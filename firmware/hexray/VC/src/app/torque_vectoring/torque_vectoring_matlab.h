#pragma once

#include "tv_debug.h" // debug bus types

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * Matlab Wrapper for update
     */
    void update_matlab(
        double    v_x,
        double    v_y,
        double    yaw_rate,
        double    a_x,
        double    a_y,
        double    apps,
        double    brake,
        double    steer,
        double    kappas[4],
        double    torque_max[4],
        double    torque_min[4],
        tv_debug *debug);

    /**
     * Matlab wrapper for kappa_update
     */
    void kappa_update_matlab(double kappas[4], double v_x, double oemgas[4]);
#ifdef __cplusplus
}
#endif
