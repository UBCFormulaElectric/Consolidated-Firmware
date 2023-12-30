#pragma once

/**
 * Trapezoidal approximation used to approximate a definite integral
 * @param integral The current value of the integral
 * @param f_x0 The previous value of f(x) used to approximate the value of the current trapezoid
 * @param f_x1 The current value of f(x) used to approximate the value of the current trapezoid
 * @param d_x The uniform delta-x of each trapezoid
 */
static inline void App_SharedProcessing_TrapezoidalRule(double *integral, float *f_prev, float f_curr, float d_x)
{
    // Calculate the trapezoid and add to the previous integral
    // Double is used here as the extra precision is needed when adding a small delta to a large integral amount
    *integral = *integral + (double)(d_x * (*f_prev + f_curr) * 0.5f);
    *f_prev   = f_curr;
}

/**
 * Linear derating algorithm
 * @param x Value on x axis
 * @param max_y Value corresponding to 100% of the y value
 * @param roll_off_x The value to begin derating the power
 * @param max_x The x value where you want y to be 0%
 * @return Linearly derated value
 */
float App_SharedProcessing_LinearDerating(float x, float max_y, float roll_off_x, float max_x);
