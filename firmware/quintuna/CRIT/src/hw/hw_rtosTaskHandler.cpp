#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<512> Task1kHz{ "Task1kHz", osPriorityRealtime, tasks_run1kHz };
static StaticTask<512> Task1Hz{ "Task1Hz", osPriorityAboveNormal, tasks_run1Hz };
static StaticTask<512> Task100Hz{ "Task100Hz", osPriorityHigh, tasks_run100Hz };
static StaticTask<512> TaskCanRx{ "TaskCanRx", osPriorityNormal, tasks_runCanRx };
static StaticTask<512> TaskCanTx{ "TaskCanTx", osPriorityNormal, tasks_runCanTx };
static StaticTask<512> TaskChimera{ "TaskChimera", osPriorityHigh, tasks_runChimera };

extern "C" inline void CRIT_StartAllTasks()
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskChimera.start();
}