#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"
#include "hw_rtosTaskHandler.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        io::time::delayUntil(start_time + 1000);
    }
}
[[noreturn]] static void tasks_run100Hz(void *arg)
{
    forever
    {
        jobs_run100Hz_tick();
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    forever
    {
        jobs_run1kHz_tick();
    }
}

[[noreturn]] static void tasks_runLogging(void *arg)
{
    forever
    {
        jobs_runLogging_tick();
    }
}
[[noreturn]] static void tasks_runTelem(void *arg)
{
    forever
    {
        jobs_runTelem_tick();
    }
}
[[noreturn]] static void tasks_runTelemRx(void *arg)
{
    forever
    {
        jobs_runTelemRx();
    }
}

[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
        jobs_runCanTx_tick();
    }
}
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        jobs_runCanRx_tick();
    }
}

// Define the task with StaticTask Class
static hw::rtos::StaticTask<8096> Task100Hz(osPriorityRealtime, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>  TaskCanTx(osPriorityAboveNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512>  TaskCanRx(osPriorityHigh, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512>  Task1kHz(osPriorityBelowNormal, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512>  Task1Hz(osPriorityBelowNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<1024> TaskLogging(osPriorityHigh, "TaskLogging", tasks_runLogging);
static hw::rtos::StaticTask<512>  TaskTelem(osPriorityHigh, "TaskTelem", tasks_runTelem);
static hw::rtos::StaticTask<512>  TaskTelemRx(osPriorityHigh, "TaskTelemRx", tasks_runTelemRx);

void DAM_StartAllTasks()
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

void tasks_preInit() {}

void tasks_init()
{
    jobs_init();
    osKernelInitialize();
    DAM_StartAllTasks();
    osKernelStart();
    forever {}
}
