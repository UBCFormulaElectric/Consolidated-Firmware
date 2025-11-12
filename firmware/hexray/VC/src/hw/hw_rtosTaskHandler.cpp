#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<8096, osPriorityHigh, "Task100Hz">     Task100Hz{ tasks_run100Hz };
static StaticTask<512, osPriorityRealtime, "Task1kHz">   Task1kHz{ tasks_run1kHz };
static StaticTask<512, osPriorityAboveNormal, "Task1Hz"> Task1Hz{ tasks_run1Hz };
static StaticTask<512, osPriorityHigh, "TaskChimera">    TaskChimera{ tasks_runChimera };

CFUNC void VC_StartAllTasks(void)
{
    Task100Hz.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskChimera.start();
}
