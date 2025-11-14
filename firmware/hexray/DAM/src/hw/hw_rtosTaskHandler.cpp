#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"
const char                                                   Task100HzName[]   = "Task100Hz";
const char                                                   TaskCanTxName[]   = "TaskCanTx";
const char                                                   TaskCanRxName[]   = "TaskCanRx";
const char                                                   Task1kHzName[]    = "Task1kHz";
const char                                                   Task1HzName[]     = "Task1Hz";
const char                                                   TaskLoggingName[] = "TaskLogging";
const char                                                   TaskTelemName[]   = "TaskTelem";
const char                                                   TaskTelemRxName[] = "TaskTelemRx";
static StaticTask<8096, osPriorityHigh, Task100HzName>       Task100Hz{ tasks_run100Hz };
static StaticTask<512, osPriorityBelowNormal, TaskCanTxName> TaskCanTx{ tasks_runCanTx };
static StaticTask<512, osPriorityBelowNormal, TaskCanRxName> TaskCanRx{ tasks_runCanRx };
static StaticTask<512, osPriorityRealtime, Task1kHzName>     Task1kHz{ tasks_run1kHz };
static StaticTask<512, osPriorityAboveNormal, Task1HzName>   Task1Hz{ tasks_run1Hz };
static StaticTask<1024, osPriorityLow, TaskLoggingName>      TaskLogging{ tasks_runLogging };
static StaticTask<512, osPriorityNormal, TaskTelemName>      TaskTelem{ tasks_runTelem };
static StaticTask<512, osPriorityLow, TaskTelemRxName>       TaskTelemRx{ tasks_runTelemRx };

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
