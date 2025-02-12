#include "io_taskMonitor.h"

/**
 * @brief  Check if the stack high watermark for a task exceeds the specified
 *         percentage threshold. If the threshold is exceeded, it may be a good
 *         idea to increase the task stack size.
 * @return True if the watermark threshold is exceeded, false otherwise
 */

float io_taskMonitor_getStackUsage(const TaskMonitor *monitor)
{
    // WARNING: osThreadGetStackSize is not implemented in FreeRTOS
    // https://github.com/ARM-software/CMSIS-FreeRTOS/blob/dd7793adcbea0c3c0f3524f86b031ab88b9e2193/DoxyGen/General/src/cmsis_freertos.txt#L312
    const float free_space = (float)osThreadGetStackSpace(*monitor->handle) / (float)monitor->attributes->stack_size;
    return 1 - free_space;
}