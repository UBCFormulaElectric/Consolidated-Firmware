#include "hw_rtosTaskHandler.hpp"

// Task initialization for RTOS tasks in the IO layer. This layer for SIL testing purposes.
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512> TaskChimera(osPriorityHigh, "TaskChimera", tasks_runChimera);

CFUNC void CRIT_StartAllTasks(void)
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskChimera.start();
}