#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"
#include "hw_rtosTaskHandler.hpp"

// Define Stack sizes with multiples of 8 bytes required by ARM
alignas(8) static uint32_t Task100HzStack[8096];
alignas(8) static uint32_t TaskCanTxStack[512];
alignas(8) static uint32_t TaskCanRxStack[512];
alignas(8) static uint32_t Task1kHzStack[512];
alignas(8) static uint32_t Task1HzStack[512];
alignas(8) static uint32_t TaskLoggingStack[1024];
alignas(8) static uint32_t TaskTelemStack[512];
alignas(8) static uint32_t TaskTelemRxStack[512];

// Define the task with StaticTask Class
static hw::rtos::StaticTask Task100Hz(Task100HzStack, 8096, osPriorityRealtime, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask TaskCanTx(TaskCanTxStack, 512, osPriorityAboveNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask TaskCanRx(TaskCanRxStack, 512, osPriorityHigh, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask Task1kHz(Task1kHzStack, 512, osPriorityBelowNormal, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask Task1Hz(Task1HzStack, 512, osPriorityBelowNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask TaskLogging(TaskLoggingStack, 1024, osPriorityHigh, "TaskLogging", tasks_runLogging);
static hw::rtos::StaticTask TaskTelem(TaskTelemStack, 512, osPriorityHigh, "TaskTelem", tasks_runTelem);
static hw::rtos::StaticTask TaskTelemRx(TaskTelemRxStack, 512, osPriorityHigh, "TaskTelemRx", tasks_runTelemRx);

void tasks_preInit() {}
void tasks_init()
{
    jobs_init();
}

void tasks_run1Hz(void *arg)
{
    forever
    {
        // TODO: figure out why io::time doesnt compile
        //  const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        // io::time::delayUntil(start_time + 1000);
    }
}
void tasks_run100Hz(void *arg)
{
    forever
    {
        jobs_run100Hz_tick();
    }
}
void tasks_run1kHz(void *arg)
{
    forever
    {
        jobs_run1kHz_tick();
    }
}

void tasks_runLogging(void *arg)
{
    forever
    {
        jobs_runLogging_tick();
    }
}
void tasks_runTelem(void *arg)
{
    forever
    {
        jobs_runTelem_tick();
    }
}
void tasks_runTelemRx(void *arg)
{
    forever
    {
        jobs_runTelemRx();
    }
}

void tasks_runCanTx(void *arg)
{
    forever
    {
        jobs_runCanTx_tick();
    }
}
void tasks_runCanRx(void *arg)
{
    forever
    {
        jobs_runCanRx_tick();
    }
}

CFUNC void DAM_StartAllTasks(void)
{
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskLogging.start();
    TaskTelem.start();
    TaskTelemRx.start();
}
