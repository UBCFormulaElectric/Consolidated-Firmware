#pragma once

typedef enum TaskIndex_e
{
    TASK_RUN100HZ          = 0,
    TASK_RUNCANTX          = 1,
    TASK_RUNCANRX          = 2,
    TASK_RUN10HZ           = 3,
    TASK_RUN1HZ            = 4,
    TASK_RUNCHIMERA        = 5,
    TASK_RUNLTCVOLTAGES    = 6,
    TASK_RUNLTCTEMPS       = 7,
    TASK_RUNLTCDIAGNOSTICS = 8,
    NUM_OF_TASKS           = 9
} TaskIndex_e;

void           tasks_preInit(void);
void           tasks_init(void);
_Noreturn void tasks_run1Hz(void);
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_runCanTx(void);
_Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_runChimera(void);
_Noreturn void tasks_runLtcVoltages(void);
_Noreturn void tasks_runLtcTemps(void);
_Noreturn void tasks_runLtcDiagnostics(void);
