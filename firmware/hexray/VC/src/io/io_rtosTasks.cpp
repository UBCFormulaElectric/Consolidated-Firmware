#include "hw_rtosTaskHandler.hpp"

// Task initialization for RTOS tasks in the IO layer. This layer for SIL testing purposes.
static hw::rtos::StaticTask<8096> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>  Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512>  Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512>  TaskChimera(osPriorityHigh, "TaskChimera", tasks_runChimera);

CFUNC void VC_StartAllTasks(void)
{
    Task100Hz.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskChimera.start();
}
