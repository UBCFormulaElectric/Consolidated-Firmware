#include <list>
#include "app_freeRtosHandler.hpp" 

extern "C" {
  void StartTask1kHz(void* argument);
  void RunTask1Hz(void* argument);
  void RunTask100Hz(void* argument);
  void RunTaskCanRx(void* argument);
  void RunTaskCanTx(void* argument);
  void RunTaskChimera(void* argument);
}

// Define tasks below 
RTOSTHREAD_DEFINE_TASK(512, "Task1kHz",    osPriorityRealtime,     StartTask1kHz,   nullptr);
RTOSTHREAD_DEFINE_TASK(512, "Task1Hz",     osPriorityAboveNormal,  RunTask1Hz,      nullptr);
RTOSTHREAD_DEFINE_TASK(512, "Task100Hz",   osPriorityHigh,         RunTask100Hz,    nullptr);
RTOSTHREAD_DEFINE_TASK(512, "TaskCanRx",   osPriorityNormal,       RunTaskCanRx,    nullptr);
RTOSTHREAD_DEFINE_TASK(512, "TaskCanTx",   osPriorityNormal,       RunTaskCanTx,    nullptr);
RTOSTHREAD_DEFINE_TASK(512, "TaskChimera", osPriorityHigh,         RunTaskChimera,  nullptr);

// C-callable hook invoked from main.c
extern "C" void RTOSTHREAD_CreateTasks(void) {
  rtosThread::create_all_tasks();
}
