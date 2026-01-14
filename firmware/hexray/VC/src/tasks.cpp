#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"
#include "hw_rtosTaskHandler.hpp"

// Define the task with StaticTask Class
static hw::rtos::StaticTask<8096> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>  Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512>  Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512>  TaskChimera(osPriorityHigh, "TaskChimera", tasks_runChimera);

void tasks_preInit() {}

void tasks_run1Hz(void *arg)
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        io::time::delayUntil(start_time + 1000);
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

void tasks_runChimera(void *arg)
{
    forever {}
}

void VC_StartAllTasks(void)
{
    Task100Hz.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskChimera.start();
}

CFUNC void tasks_init()
{
    jobs_init();
    osKernelInitialize();
    VC_StartAllTasks();
    osKernelStart();
}
