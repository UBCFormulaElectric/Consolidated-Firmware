#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<8096, osPriorityHigh, "Task100Hz">       Task100Hz{ "Task100Hz", osPriorityHigh, tasks_run100Hz };
static StaticTask<512, osPriorityBelowNormal, "TaskCanTx"> TaskCanTx{ "TaskCanTx", osPriorityBelowNormal,
                                                                      tasks_runCanTx };
static StaticTask<512, osPriorityBelowNormal, "TaskCanRx"> TaskCanRx{ "TaskCanRx", osPriorityBelowNormal,
                                                                      tasks_runCanRx };
static StaticTask<512, osPriorityRealtime, "Task1kHz">     Task1kHz{ "Task1kHz", osPriorityRealtime, tasks_run1kHz };
static StaticTask<512, osPriorityAboveNormal, "Task1Hz">   Task1Hz{ "Task1Hz", osPriorityAboveNormal, tasks_run1Hz };
static StaticTask<1024, osPriorityLow, "TaskLogging"> TaskLogging{ "TaskLogging", osPriorityLow, tasks_runLogging };
static StaticTask<512, osPriorityNormal, "TaskTelem"> TaskTelem{ "TaskTelem", osPriorityNormal, tasks_runTelem };
static StaticTask<512, osPriorityLow, "TaskTelemRx">  TaskTelemRx{ "TaskTelemRx", osPriorityLow, tasks_runTelemRx };

extern "C" void DAM_StartAllTasks(void)
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
