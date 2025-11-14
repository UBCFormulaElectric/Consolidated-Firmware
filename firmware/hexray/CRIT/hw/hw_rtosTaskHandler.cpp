#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"
const char Task1kHzName[] = "Task1kHz";
const char Task1HzName[] = "Task1Hz";
const char Task100HzName[] = "Task100Hz";
const char TaskCanRxName[] = "TaskCanRx";
const char TaskCanTxName[] = "TaskCanTx";
const char TaskChimeraName[] = "TaskChimera";

static StaticTask<512, osPriorityRealtime, Task1kHzName>   Task1kHz {tasks_run1kHz};
static StaticTask<512, osPriorityAboveNormal,Task1HzName> Task1Hz{tasks_run1Hz};
static StaticTask<512, osPriorityHigh, Task100HzName>      Task100Hz{tasks_run100Hz};
static StaticTask<512, osPriorityNormal, TaskCanRxName>    TaskCanRx{tasks_runCanRx};
static StaticTask<512, osPriorityNormal, TaskCanTxName>    TaskCanTx{tasks_runCanTx};
static StaticTask<512, osPriorityHigh, TaskChimeraName>    TaskChimera{tasks_runChimera};

CFUNC void CRIT_StartAllTasks(void)
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskChimera.start();
}