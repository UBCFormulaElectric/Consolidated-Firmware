#pragma once

/** @brief The information needed to check the stack watermark of a task. */
#ifdef TARGET_EMBEDDED
#include <cmsis_os.h>
typedef struct
{
    osThreadId_t *handle; // We can't use TaskHandle as a compile-time constant,
                          // so instead we use a pointer to it.
    const osThreadAttr_t *attributes;
} TaskMonitor;
#else
#include "app_utils.h"
EMPTY_STRUCT(TaskMonitor);
#endif

/**
 * @brief Check the stack high watermark for FreeRTOS tasks.
 *        Typically, you would call this function at 1Hz to minimize overhead.
 */
float io_taskMonitor_getStackUsage(const TaskMonitor *monitor);
