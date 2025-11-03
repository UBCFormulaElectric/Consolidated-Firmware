#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<8096, osPriorityHigh, "Task100Hz"> Task100Hz{ tasks_run100Hz };
static StaticTask<512, osPriorityBelowNormal, "TaskCan1Tx"> TaskCan1Tx{ tasks_runCan1Tx };
static StaticTask<512, osPriorityBelowNormal, "TaskCanRx"> TaskCanRx{ tasks_runCanRx };
static StaticTask<512, osPriorityRealtime, "Task1kHz"> Task1kHz{  tasks_run1kHz };
static StaticTask<512, osPriorityAboveNormal, "Task1Hz"> Task1Hz{ tasks_run1Hz };
static StaticTask<512, osPriorityLow, "TaskBtrMonitor"> TaskBtrMonitor{ tasks_batteryMonitoring };
static StaticTask<512, osPriorityHigh, "TaskChimera"> TaskChimera{ tasks_runChimera };
static StaticTask<512, osPriorityBelowNormal, "TaskCan2Tx"> TaskCan2Tx{ tasks_runCan2Tx };
static StaticTask<512, osPriorityBelowNormal, "TaskCan3Tx"> TaskCan3Tx{ tasks_runCan3Tx };
static StaticTask<512, osPriorityAboveNormal2, "TaskPwrMontr"> TaskCanTx{ tasks_powerMonitoring };

extern "C" void VC_StartAllTasks(void)
{
    Task100Hz.start();
    TaskCan1Tx.start();
    TaskCanRx.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskBtrMonitor.start();
    TaskChimera.start();
    TaskCan2Tx.start();
    TaskCan3Tx.start();
    TaskPwrMontr.start();
}
