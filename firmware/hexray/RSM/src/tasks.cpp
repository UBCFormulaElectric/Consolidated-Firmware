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
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run100Hz_tick();
        io::time::delayUntil(start_time + 10);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1kHz_tick();
        io::time::delayUntil(start_time + 10);
    }
}
// [[noreturn]] static void tasks_runCanTx(void *arg)
// {
//     forever
//     {
//         jobs_runCanTx_tick();
//     }
// }
// [[noreturn]] static void tasks_runCanRx(void *arg)
// {
//     forever
//     {
//         jobs_runCanRx_tick();
//     }
// }

// Define the task with StaticTask Template Class
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
// static hw::rtos::StaticTask<512> TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx);
// static hw::rtos::StaticTask<512> TaskCanRx(osPriorityLow, "TaskCanRx", tasks_runCanRx);

static void RSM_StartAllTasks()
{
    Task1kHz.start();
    Task100Hz.start();
    // TaskCanTx.start();
    // TaskCanRx.start();
    Task1Hz.start();
}

void tasks_preInit() {}

void tasks_init()
{
    jobs_init();
    osKernelInitialize();
    RSM_StartAllTasks();
    osKernelStart();
    forever {}
}