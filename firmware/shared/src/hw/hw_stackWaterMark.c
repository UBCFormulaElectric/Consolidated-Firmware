#include <assert.h>
#include "hw_stackWaterMark.h"

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
static bool stackAboveWatermarkThreshold(TaskHandle_t xTask, uint32_t stack_size, float watermark_threshold);

static bool stackAboveWatermarkThreshold(TaskHandle_t xTask, uint32_t stack_size, float watermark_threshold)
{
    assert((0.0f < watermark_threshold) && (watermark_threshold < 1.0f));

    bool is_stack_above_watermark_threshold = false;

    float stack_high_watermark_percent = 1.0f - (float)uxTaskGetStackHighWaterMark(xTask) / (float)stack_size;

    if (stack_high_watermark_percent > watermark_threshold)
    {
        is_stack_above_watermark_threshold = true;
    }

    return is_stack_above_watermark_threshold;
}

void hw_stackWaterMark_check(StackWaterMark *stacks, size_t num_of_stacks)
{
    for (size_t i = 0; i < num_of_stacks; i++)
    {
        if (stackAboveWatermarkThreshold(*(stacks[i].handle), stacks[i].stack_size, stacks[i].watermark_threshold))
        {
            stacks[i].log_error(1);
        }
    }
}

static float hw_stackSize(TaskHandle_t xTask, uint32_t stack_size, float watermark_threshold)
{
    assert((0.0f < watermark_threshold) && (watermark_threshold < 1.0f));

    float stack_high_watermark_percent_remaining =
        100.0f * (float)uxTaskGetStackHighWaterMark(xTask) / (float)stack_size;

    return stack_high_watermark_percent_remaining;
}

void hw_stackSize_check(struct stack_watermark *stacks, size_t num_of_stacks)
{
    for (size_t i = 0; i < num_of_stacks; i++)
    {
        stacks[i].stack_remaining(
            hw_stackSize(*(stacks[i].handle), stacks[i].stack_size, stacks[i].watermark_threshold));
    }
}
