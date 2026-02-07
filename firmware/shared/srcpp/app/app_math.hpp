#pragma once
#include <cmath>
#include <algorithm>

#define APPROX_EQUAL_FLOAT(a, b, threshold) ((bool)(std::fabs((a) - (b)) < threshold))
#define IS_IN_RANGE(min, max, val) (((val) > (min)) && ((val) < (max)))
#define CLAMP(x, min, max) std::clamp(x, min, max);
#define REDUCE_X true
#define INCREASE_X false

namespace app::math
{

/**
 * Trapezoidal approximation used to approximate a definite integral
 * @param integral Reference to the current value of the integral
 * @param f_prev Reference to the previous value of f(x)
 * @param f_curr The current value of f(x)
 * @param d_x The uniform delta-x of each trapezoid
 */
inline void trapezoidalRule(double &integral, float &f_prev, const float f_curr, const double d_x)
{
    // Calculate the trapezoid and add to the previous integral
    integral += (d_x * static_cast<double>(f_prev + f_curr) * 0.5);

    // Update previous value for the next iteration
    f_prev = f_curr;
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
float linearDerating(float x, float max_y, float roll_off_x, float limit_x, bool derating_direction);

} // namespace app::math
