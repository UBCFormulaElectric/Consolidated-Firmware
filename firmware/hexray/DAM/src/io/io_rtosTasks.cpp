#include "hw_rtosTaskHandler.hpp"

// Task initialization for RTOS tasks in the IO layer. This layer for SIL testing purposes.
static hw::rtos::StaticTask<8096> Task100Hz(osPriorityRealtime, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>  TaskCanTx(osPriorityAboveNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512>  TaskCanRx(osPriorityHigh, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512>  Task1kHz(osPriorityBelowNormal, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512>  Task1Hz(osPriorityBelowNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<1024> TaskLogging(osPriorityHigh, "TaskLogging", tasks_runLogging);
static hw::rtos::StaticTask<512>  TaskTelem(osPriorityHigh, "TaskTelem", tasks_runTelem);
static hw::rtos::StaticTask<512>  TaskTelemRx(osPriorityHigh, "TaskTelemRx", tasks_runTelemRx);

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
