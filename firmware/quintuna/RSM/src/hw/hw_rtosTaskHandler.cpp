#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<512, osPriorityRealtime, "Task1kHz"> Task1kHz{ "Task1kHz", osPriorityRealtime, tasks_run1kHz };
static StaticTask<512, osPriorityHigh, "Task100Hz"> Task100Hz{ "Task100Hz", osPriorityHigh, tasks_run100Hz };
static StaticTask<512, osPriorityAboveNormal, "Task1Hz"> Task1Hz{ "Task1Hz", osPriorityAboveNormal, tasks_run1Hz };
static StaticTask<512, osPriorityNormal, "TaskCanTx"> TaskCanTx{ "TaskCanTx", osPriorityNormal, tasks_runCanTx };
static StaticTask<512, osPriorityLow, "TaskCanRx"> TaskCanRx{ "TaskCanRx", osPriorityLow, tasks_runCanRx };
static StaticTask<512, osPriorityHigh, "TaskChimera"> TaskChimera{ "TaskChimera", osPriorityHigh, tasks_runChimera };

extern "C" inline void RSM_StartAllTasks()
{
    Task1kHz.start();
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1Hz.start();
    TaskChimera.start();
}