#pragma once
#include "stdbool.h"

// Direction to derate x variable
#define REDUCE_X true
#define INCREASE_X false

/**
 * Trapezoidal approximation used to approximate a definite integral
 * @param integral The current value of the integral
 * @param f_x0 The previous value of f(x) used to approximate the value of the current trapezoid
 * @param f_x1 The current value of f(x) used to approximate the value of the current trapezoid
 * @param d_x The uniform delta-x of each trapezoid
 */
static inline void app_math_trapezoidalRule(double *integral, float *f_prev, const float f_curr, const double d_x)
{
    // Calculate the trapezoid and add to the previous integral
    // Double is used here as the extra precision is needed when adding a small delta to a large integral amount
    *integral = *integral + (d_x * (double)(*f_prev + f_curr) * 0.5);
    *f_prev   = f_curr;
}

/**
 * Linear derating algorithm
 * @param x Value on x axis
 * @param max_y Value corresponding to 100% of the y value
 * @param roll_off_x The value to begin derating the power
 * @param limit_x The x value where you want y to be 0%
 * @param derating_direction True if x needs to be decreased, False if x needs to be increased
 * @return Linearly derated value
 */
float app_math_linearDerating(float x, float max_y, float roll_off_x, float limit_x, bool derating_direction);
