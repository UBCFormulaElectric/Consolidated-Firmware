#include "control/App_PIController.h"
#include "configs/App_ControlSystemConfig.h"

/*
 * PI controller with anti-windup and compensation
 */

// calculates PI outputs for Id, Iq or speed control loops
void calculatePiOutputs(
    struct ControllerValues *pi_values,
    const float              ref,
    const float              actual,
    const float              limit,
    const float              comp)
{
    float err       = ref - actual;
    float prop_term = pi_values->gain * err;
    if (!(((pi_values->integral_sum + prop_term + comp) * err > 0) &&
          ((pi_values->integral_sum + prop_term + comp > limit) ||
           (pi_values->integral_sum + prop_term + comp < -limit))))
    {
        // y(n) = y(n-1) + T/2 * (e(n) + e(n-1)) Average of error and last error *
        // sampling frequency + last integral input
        pi_values->integral_sum += (1.0f / SAMPLE_FREQUENCY) *
                (pi_values->gain / pi_values->time_const) * (err + pi_values->prev_error) / 2;
        pi_values->prev_error = (err + pi_values->prev_error) / 2;
    }
    else if (pi_values->integral_sum > limit)
    {
        pi_values->integral_sum = limit;
    }
    else if (pi_values->integral_sum < -limit)
    {
        pi_values->integral_sum = -limit;
    }

    float output = prop_term + pi_values->integral_sum + comp;
    if ((output < limit) && (output > -limit))
    {
        pi_values->output = output;
    }
    else
    {
        pi_values->output = (output > limit) ? limit : -limit;
    }
}
