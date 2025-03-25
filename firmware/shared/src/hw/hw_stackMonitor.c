#include "hw_stackMonitor.h"
#include <assert.h>
#include <cmsis_os2.h>
#include <stdint.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    assert(0);
}

static float stackHighWatermarkPercent(uint32_t task)
{
    // WARNING: osThreadGetStackSize is not implemented in FreeRTOS
    // https://github.com/ARM-software/CMSIS-FreeRTOS/blob/dd7793adcbea0c3c0f3524f86b031ab88b9e2193/DoxyGen/General/src/cmsis_freertos.txt#L312
    const float free_space = (float)osThreadGetStackSpace(stack_monitor_config.tasks[task].handle) /
                             (float)(stack_monitor_config.tasks[task].attributes->stack_size);
    return 1.0f - free_space;
}

void hw_stackMonitor_check(void)
{
    for (uint32_t task = 0; task < stack_monitor_config.num_tasks; task++)
    {
        const float high_watermark = stackHighWatermarkPercent(task);

        if (high_watermark > STACK_HIGH_CALLBACK_THRESHOLD)
        {
            assert(stack_monitor_config.watermark_exceeded_callback);
            stack_monitor_config.watermark_exceeded_callback(stack_monitor_config.tasks[task].id);
        }
    }
}
