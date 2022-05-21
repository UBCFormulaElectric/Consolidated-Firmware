#pragma once

/**
 * Trapezoidal approximation used to approximate a definite integral
 * @param integral The current value of the integral
 * @param f_x0 The previous value of f(x) used to approximate the value of the current trapezoid
 * @param f_x1 The current value of f(x) used to approximate the value of the current trapezoid
 * @param d_x The uniform delta-x of each trapezoid
 */
static inline void App_SharedProcessing_TrapezoidalRule(float *integral, float *f_prev, float f_curr, float d_x)
{
    // Calculate the trapezoid and add to the previous integral
    *integral = *integral + d_x * (*f_prev + f_curr) * 0.5f;
    *f_prev   = f_curr;
}
