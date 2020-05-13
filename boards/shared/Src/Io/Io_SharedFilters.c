#include "Io_SharedFilters.h"
#include "arm_math.h"

void Io_SharedFilters_LowPassFilter(
    float32_t *input,
    float32_t *output,
    uint32_t   num_of_samples,
    float32_t  sampling_time,
    float32_t  rc)
{
    float32_t smoothing_factor;

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

void Filter_ADC_Readings(uint32_t ChannelCount, uint32_t FilterSize, uint16_t *ADC_Readings, float *Filtered_ADC_Readings)
{
    size_t i;
    size_t j;

    // Reset filtered ADC readings array to 0
    for(i = 0; i < ChannelCount; i++)
    {
        Filtered_ADC_Readings[i] = 0;
    }

    // Calculate sum of previous ADC_FILTER_SIZE readings
    for(i = 0; i < FilterSize; i++)
    {
        for(j = 0; j < ChannelCount; j++)
        {
            Filtered_ADC_Readings[j] += ADC_Readings[(ChannelCount * i) + j];
        }
    }

    // Calculate average of previous ADC_FILTER_SIZE readings
    for(i = 0; i < ChannelCount; i++)
    {
        Filtered_ADC_Readings[i] = Filtered_ADC_Readings[i] / (float)FilterSize;
    }
}
