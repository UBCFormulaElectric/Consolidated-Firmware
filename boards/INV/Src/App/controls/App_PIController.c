#include "controls/App_PIController.h"
#include "configs/App_ControlSystemConfig.h"

/*
 * PI controller with anti-windup and compensation
 */

// calculates PI outputs for Id, Iq or speed control loops
double calculatePiOutputs(
    struct ControllerValues *pi_values,
    const double             ref,
    const double             actual,
    const double             limit,
    const double             comp)
{
    double err       = ref - actual;
    double prop_term = pi_values->gain * err;
    if (!(((pi_values->integral_sum + prop_term + comp) * err > 0) &&
          ((pi_values->integral_sum + prop_term + comp > limit) ||
           (pi_values->integral_sum + prop_term + comp < -limit))))
    {
        // y(n) = y(n-1) + T/2 * (u(n) + u(n-1)) Average of current and last *
        // sampling frequency + last integral input
        pi_values->integral_sum =
            pi_values->integral_sum +
            (1 / SAMPLE_FREQUENCY) *
                ((err * pi_values->gain / pi_values->time_const) +
                 pi_values->prev_integral_input) /
                2;
        pi_values->prev_integral_input =
            (1 / SAMPLE_FREQUENCY) *
            ((err * pi_values->gain / pi_values->time_const) +
             pi_values->prev_integral_input) /
            2;
    }

    double output = prop_term + pi_values->integral_sum + comp;
    if ((output < limit) && (output > -limit))
    {
        return output;
    }
    else
    {
        return (output > limit) ? limit : -limit;
    }
}
