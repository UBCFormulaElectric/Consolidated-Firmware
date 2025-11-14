#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

const char                                                 Task100HzName[]   = "Task100Hz";
const char                                                 Task1kHzName[]    = "Task1kHz";
const char                                                 Task1HzName[]     = "Task1Hz";
const char                                                 TaskChimeraName[] = "TaskChimera";
static StaticTask<8096, osPriorityHigh, Task100HzName>     Task100Hz{ tasks_run100Hz };
static StaticTask<512, osPriorityRealtime, Task1kHzName>   Task1kHz{ tasks_run1kHz };
static StaticTask<512, osPriorityAboveNormal, Task1HzName> Task1Hz{ tasks_run1Hz };
static StaticTask<512, osPriorityHigh, TaskChimeraName>    TaskChimera{ tasks_runChimera };

CFUNC void VC_StartAllTasks(void)
{
    Task100Hz.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskChimera.start();
}
