#pragma once

#include <cmsis_os.h>

#define MAX_NUM_TASKS (10)
#define STACK_HIGH_CALLBACK_THRESHOLD (0.70f)

typedef struct
{
    uint32_t num_tasks;
    struct
    {
        osThreadId_t         *handle;
        const osThreadAttr_t *attributes;
        uint32_t              id;
    } tasks[MAX_NUM_TASKS];
    void (*watermark_exceeded_callback)(uint32_t id);
} StackMonitorConfig;

extern const StackMonitorConfig stack_monitor_config;

void hw_stackMonitor_check(void);
