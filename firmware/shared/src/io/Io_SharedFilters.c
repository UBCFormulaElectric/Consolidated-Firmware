#include "Io_SharedFilters.h"

void Io_SharedFilters_LowPassFilter(float* input, float* output, uint32_t num_of_samples, float sampling_time, float rc)
{
    float smoothing_factor;

    smoothing_factor = sampling_time / (rc + sampling_time);

    // The pseudo-code for this LPF implementation is as follows:
    // y[i] = y[i-1] + SmoothingFactor * ( x[i] - y[i-1] ), where y =
    // output, x = input. That is, the change from one filter output
    // to the next is proportional to the difference between the previous
    // output and the next input.
    for (uint32_t i = 0; i < num_of_samples; i++)
    {
        output[i] = output[i] + smoothing_factor * (input[i] - output[i]);
    }
}
