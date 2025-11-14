#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"
static StaticTask<512, osPriorityRealtime>    Task1kHz("Task1kHz", tasks_run1kHz);
static StaticTask<512, osPriorityAboveNormal> Task1Hz("Task1Hz", tasks_run1Hz);
static StaticTask<512, osPriorityHigh>        Task100Hz("Task100Hz", tasks_run100Hz);
static StaticTask<512, osPriorityNormal>      TaskCanRx("TaskCanRx", tasks_runCanRx);
static StaticTask<512, osPriorityNormal>      TaskCanTx("TaskCanTx", tasks_runCanTx);
// static StaticTask<512, osPriorityHigh>        TaskChimera("TaskChimera", tasks_runChimera);

extern "C" void CRIT_StartAllTasks(void)
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    //   TaskChimera.start();
}