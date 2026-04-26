#pragma once

#include "io_canMsg.h"

typedef enum TaskIndex_e
{
    TASK_RUN100HZ   = 0,
    TASK_RUNCANTX   = 1,
    TASK_RUNCANRX   = 2,
    TASK_RUN10HZ    = 3,
    TASK_RUN1HZ     = 4,
    TASK_RUN1KHZ    = 5,
    TASK_RUNCHIMERA = 6,
    NUM_OF_TASKS    = 7
} TaskIndex_e;

void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run1Hz();
_Noreturn void tasks_run100Hz();
_Noreturn void tasks_run1kHz();
_Noreturn void tasks_runCanTx();
void           tasks_runCanRxCallback(const CanMsg *msg);
_Noreturn void tasks_runCanRx();
_Noreturn void tasks_runChimera(void);