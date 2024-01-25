#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include <stdbool.h>

/** @brief The information needed to check the stack watermark of a task. */
struct stack_watermark
{
    TaskHandle_t *handle; // We can't use TaskHandle as a compile-time constant,
                          // so instead we use a pointer to it.
    uint32_t stack_size;
    float    watermark_threshold;
    void (*log_error)(uint8_t);
};

/**
 * @brief Check the stack high watermark for FreeRTOS tasks. Typically, you
 *        would call this function at 1Hz to minimize overhead. If any stack
 *        watermark threshold is exceeded, we log the error.
 * @param stacks An array of stacks whose watermarks we are checking for.
 * @param num_of_stacks Number of stacks in the array.
 */
void Io_SharedStackWaterMark_Check(struct stack_watermark *stacks, size_t num_of_stacks);
