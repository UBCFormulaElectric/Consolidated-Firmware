#include "World/App_SharedWorld.h"
#include "App_SharedAssert.h"
#include "Io_SharedStackWaterMark.h"

extern struct World *world;

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
static bool App_IsStackAboveWatermarkThreshold(
    TaskHandle_t xTask,
    uint32_t     stack_size,
    float        watermark_threshold);

static bool App_IsStackAboveWatermarkThreshold(
    TaskHandle_t xTask,
    uint32_t     stack_size,
    float        watermark_threshold)
{
    shared_assert((0.0 < watermark_threshold) && (watermark_threshold < 1.0));

    bool is_stack_above_watermark_threshold = false;

    float stack_high_watermark_percent =
        1.0f - (float)uxTaskGetStackHighWaterMark(xTask) / (float)stack_size;

    if (stack_high_watermark_percent > watermark_threshold)
    {
        is_stack_above_watermark_threshold = true;
    }

    return is_stack_above_watermark_threshold;
}

void Io_SharedStackWaterMark_Check(
    struct stack_watermark *stacks,
    size_t                  num_of_stacks)
{
    shared_assert(stacks != NULL);

    for (size_t i = 0; i < num_of_stacks; i++)
    {
        shared_assert(*(stacks[i].handle) != NULL);
        shared_assert(stacks[i].log_error != NULL);

        if (App_IsStackAboveWatermarkThreshold(
                *(stacks[i].handle), stacks[i].stack_size,
                stacks[i].watermark_threshold) == true)
        {
            stacks[i].log_error(App_SharedWorld_GetCanTx(world), 1);
        }
    }
}
