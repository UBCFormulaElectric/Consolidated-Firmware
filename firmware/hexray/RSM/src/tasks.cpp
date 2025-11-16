#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"
#include "hw_rtosTaskHandler.hpp"

// Define Stack sizes with multiples of 8 bytes required by ARM
alignas(8) static uint32_t Task1kHzStack[512];
alignas(8) static uint32_t Task100HzStack[512];
alignas(8) static uint32_t Task1HzStack[512];
alignas(8) static uint32_t TaskCanTxStack[512];
alignas(8) static uint32_t TaskCanRxStack[512];
alignas(8) static uint32_t TaskChimeraStack[512];

// Define the task with StaticTask Class
static hw::rtos::StaticTask Task1kHz(Task1kHzStack, 512, osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask Task100Hz(Task100HzStack, 512, osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask Task1Hz(Task1HzStack, 512, osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask TaskCanTx(TaskCanTxStack, 512, osPriorityNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask TaskCanRx(TaskCanRxStack, 512, osPriorityLow, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask TaskChimera(TaskChimeraStack, 512, osPriorityHigh, "TaskChimera", tasks_runChimera);

void tasks_preInit() {}
void tasks_init()
{
    jobs_init();
}

void tasks_run1Hz(void *arg)
{
    forever
    {
        // const uint32_t start_time = io::time::getCurrentMs();
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
void tasks_runChimera(void *arg)
{
    forever
    {
        jobs_runChimera_tick();
    }
}

CFUNC void RSM_StartAllTasks(void)
{
    Task1kHz.start();
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1Hz.start();
    TaskChimera.start();
}