/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedFilters.h"
#include "arm_math.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedFilters_LowPassFilter(
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

    output[0] = smoothing_factor * input[0];

    for (uint32_t i = 1; i < num_of_samples; i++)
    {
        output[i] = output[i-1] + smoothing_factor * (input[i] - output[i-1]);
    }
}
