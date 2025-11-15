#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

// Define names as a char array 
const char Task1kHzName[]    = "Task1kHz";
const char Task1HzName[]     = "Task1Hz";
const char Task100HzName[]   = "Task100Hz";
const char TaskCanRxName[]   = "TaskCanRx";
const char TaskCanTxName[]   = "TaskCanTx";
const char TaskChimeraName[] = "TaskChimera";

// Define Stack sizes with multiples of 8 bytes required by ARM
alignas(8) static uint32_t Task1kHzStack[512];
alignas(8) static uint32_t Task1HzStack[512];
alignas(8) static uint32_t Task100HzStack[512];
alignas(8) static uint32_t TaskCanRxStack[512];
alignas(8) static uint32_t TaskCanTxStack[512];
alignas(8) static uint32_t TaskChimeraStack[512];

// Define the task with StaticTask Class
static hw::rtos::StaticTask Task1kHz(Task1kHzStack, 512, osPriorityRealtime, Task1kHzName, tasks_run1kHz);
static hw::rtos::StaticTask Task1Hz(Task1HzStack, 512, osPriorityAboveNormal, Task1kHzName, tasks_run1Hz);
static hw::rtos::StaticTask Task100Hz(Task100HzStack, 512, osPriorityHigh, Task1kHzName, tasks_run100Hz);
static hw::rtos::StaticTask TaskCanRx(TaskCanRxStack, 512, osPriorityNormal, Task1kHzName, tasks_runCanRx);
static hw::rtos::StaticTask TaskCanTx(TaskCanTxStack, 512, osPriorityNormal, Task1kHzName, tasks_runCanTx);
static hw::rtos::StaticTask TaskChimera(TaskChimeraStack, 512, osPriorityHigh, Task1kHzName, tasks_runChimera);

CFUNC void CRIT_StartAllTasks(void)
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskChimera.start();
}