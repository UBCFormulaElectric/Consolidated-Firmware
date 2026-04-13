#pragma once

extern "C"
{
    void update_matlab(
        double v_x,
        double v_y,
        double yaw_rate,
        double a_x,
        double a_y,
        double apps,
        double delta_fl,
        double delta_fr,
        double out[6]);

    void kappa_update_matlab(double kappas[4], double v_x, double out[4]);
}