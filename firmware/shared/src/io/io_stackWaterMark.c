#include "io_stackWaterMark.h"

#include <assert.h>
#include <stdbool.h>

/**
 * @brief  Check if the stack high watermark for a task exceeds the specified
 *         percentage threshold. If the threshold is exceeded, it may be a good
 *         idea to increase the task stack size.
 * @param  xTask The task we are checking the stack water mark for
 * @param  stack_size Stack size for the task
 * @param  watermark_threshold The maximum stack high watermark we tolerate,
 *         specified as a percentage
 * @return True if the watermark threshold is exceeded, false otherwise
 */
// ReSharper disable once CppParameterMayBeConst
static bool stackAboveWatermarkThreshold(osThreadId_t xTask, const uint32_t stack_size, const float watermark_threshold)
{
    assert(0.0f < watermark_threshold && watermark_threshold < 1.0f);
    // WARNING: osThreadGetStackSize is not implemented in FreeRTOS
    // https://github.com/ARM-software/CMSIS-FreeRTOS/blob/dd7793adcbea0c3c0f3524f86b031ab88b9e2193/DoxyGen/General/src/cmsis_freertos.txt#L312
    const float free_space = (float)osThreadGetStackSpace(xTask) / (float)stack_size;
    return 1 - free_space > watermark_threshold;
}

void io_stackWaterMark_check(const StackWaterMark *stacks, const size_t num_of_stacks)
{
    for (size_t i = 0; i < num_of_stacks; i++)
        if (stackAboveWatermarkThreshold(stacks[i].handle, stacks[i].stack_size, stacks[i].watermark_threshold))
            stacks[i].log_error(1);
}
