#include "app_rtos_task.hpp"

extern "C" void TaskFn1kHz(void* arg);
extern "C" void TaskFn1Hz(void* arg);
extern "C" void TaskFn100Hz(void* arg);
extern "C" void TaskFnRx(void* arg);
extern "C" void TaskFnTx(void* arg);
extern "C" void TaskFnChimera(void* arg);


//if we want I can also move the rest of the board tasks into here and move this file to the shared file.
//after kernel is initialized you can put the line Task1kHz.start() etc. 
static StaticTask<512> Task1kHz{ "Task1kHz", osPriorityRealtime, TaskFn1kHz };
static StaticTask<512> Task1Hz{ "Task1Hz", osPriorityAboveNormal, TaskFn1Hz};
static StaticTask<512> Task100Hz{ "Task100Hz", osPriorityHigh, TaskFn100Hz};
static StaticTask<512> TaskCanRx{ "TaskCanRx", osPriorityNormal, TaskFnRx};
static StaticTask<512> TaskCanTx{ "TaskCanTx", osPriorityNormal, TaskFnTx};
static StaticTask<512> TaskChimera{ "TaskChimera", osPriorityHigh, TaskFnChimera};

inline void CRIT_StartAllTasks() {
  Task1kHz.start();
  Task1Hz.start();
  Task100Hz.start();
  TaskCanRx.start();
  TaskCanTx.start();
  TaskChimera.start();
}

// C-callable for main.c
extern "C" inline void CRIT_StartAllTasks(void) { CRIT_StartAllTasks(); }
