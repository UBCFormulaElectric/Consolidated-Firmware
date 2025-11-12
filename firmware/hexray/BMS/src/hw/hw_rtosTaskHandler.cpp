#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<512, osPriorityHigh, "Task100Hz">        Task100Hz{ "Task100Hz", osPriorityHigh, tasks_run100Hz };
static StaticTask<512, osPriorityBelowNormal, "TaskCanRx"> TaskCanRx{ "TaskCanRx", osPriorityBelowNormal,
                                                                      tasks_runCanRx };
static StaticTask<512, osPriorityBelowNormal, "TaskCanTx"> TaskCanTx{ "TaskCanTx", osPriorityBelowNormal,
                                                                      tasks_runCanTx };
static StaticTask<512, osPriorityRealtime, "Task1kHz">     Task1kHz{ "Task1kHz", osPriorityRealtime, tasks_run1kHz };
static StaticTask<512, osPriorityAboveNormal, "Task1Hz">   Task1Hz{ "Task1Hz", osPriorityAboveNormal, tasks_run1Hz };
static StaticTask<512, osPriorityHigh, "TaskChimera"> TaskChimera{ "TaskChimera", osPriorityHigh, tasks_runChimera };
static StaticTask<512, osPriorityNormal, "TaskLtcVoltages"> TaskLtcVoltages{ "TaskLtcVoltages", osPriorityNormal,
                                                                             tasks_runLtcVoltages };
static StaticTask<512, osPriorityNormal, "TaskLtcTemps">    TaskLtcTemps{ "TaskLtcTemps", osPriorityNormal,
                                                                       tasks_runLtcTemps };
static StaticTask<512, osPriorityNormal, "TaskLtcDiag">     TaskLtcDiag{ "TaskLtcDiag", osPriorityNormal,
                                                                     tasks_runLtcDiagnostics };

CFUNC void BMS_StartAllTasks(void)
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskChimera.start();
    TaskLtcVoltages.start();
    TaskLtcTemps.start();
    TaskLtcDiag.start();
}
