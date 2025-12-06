#include "app_pid.hpp"
#include <stdexcept>
#include <assert.h>

namespace app
{
PID::PID(const Config &conf)
  : Kp(conf.Kp),
    Ki(conf.Ki),
    Kd(conf.Kd),
    Kb(conf.Kb),
    Kff(conf.Kff),
    smoothing_coeff(conf.smoothing_coeff),
    out_max(conf.out_max),
    out_min(conf.out_min),
    max_integral(conf.max_integral),
    min_integral(conf.min_integral),
    clamp_output(conf.clamp_output),
    clamp_integral(conf.clamp_integral),
    back_calculation(conf.back_calculation),
    feed_forward(conf.feed_forward),
    sample_time(conf.sample_time)
{
    assert(out_max >= out_min);
    assert(max_integral >= min_integral);
    assert(sample_time > 0.0f);
}

/**
 * PID controller effort implementation with derivative-on-error and discrete time handling. Includes:
 * - Integral anti-wind up in the form of clamping and optional back calculation (requires valid min and max output
 * values)
 * - Derivative Filtering in the form of first order exponential smoothing
 * - Static Gain feed forward model
 * @param setpoint setpoint
 * @param input also "measurement"/"process variable"
 * @param disturbance feedforward/"disturbance" variable
 * @return controller output/"effort"
 */

float PID::compute(float setpoint, float input, float disturbance = 0.0f)
{
    error = setpoint - input;

    integral += error * sample_time;

    // First order exponential smoothing (https://en.wikipedia.org/wiki/Exponential_smoothing)
    float raw_derivative = (error - prev_error) / sample_time;
    derivative           = smoothing_coeff * raw_derivative + (1.0f - smoothing_coeff) * prev_derivative;
    derivative           = smoothing_coeff * raw_derivative + (1.0f - smoothing_coeff) * prev_derivative;

    // Feed Forward
    float u_ff = 0.0f;
    if (feed_forward)
    {
        u_ff = Kff * disturbance;
    }

    float output = Kp * error + Ki * integral + Kd * derivative + u_ff;

    // Anti-Windup with back calculation (https://www.cds.caltech.edu/~murray/books/AM08/pdf/am08-pid_04Mar10.pdf)
    // equation 10.16
    if (back_calculation)
    {
        float out_clamp = std::clamp(output, out_min, out_max);
        integral += Kb * (out_clamp - output);
    }

    if (clamp_output)
    {
        output = std::clamp(output, out_min, out_max);
    }
    // Conditional Anti-Windup
    if (clamp_integral)
    {
        integral = std::clamp(integral, min_integral, max_integral);
    }

    prev_error       = error;
    prev_disturbance = disturbance;
    prev_derivative  = derivative;

    return output;
}

void PID::reset()
{
    integral         = 0.0f;
    prev_derivative  = 0.0f;
    prev_disturbance = 0.0f;
    prev_error       = 0.0f;
}

float PID::getIntegral()
{
    return integral;
}

float PID::getDerivative()
{
    return derivative;
}

}
