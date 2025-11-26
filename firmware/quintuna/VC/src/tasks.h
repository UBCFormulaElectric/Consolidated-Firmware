#pragma once

typedef enum TaskIndex_e
{
    TASK_RUN1HZ            = 0,
    TASK_RUNCHIMERA        = 1,
    TASK_RUN100HZ          = 2,
    TASK_RUN1KHZ           = 3,
    TASK_RUNCAN1TX         = 4,
    TASK_RUNCAN2TX         = 5,
    TASK_RUNCAN3TX         = 6,
    TASK_RUNCANRX          = 7,
    TASK_BATTERYMONITORING = 8,
    TASK_POWERMONITORING   = 9,
    NUM_OF_TASKS           = 10,
} TaskIndex_e;

void           tasks_preInitWatchdog(void);
void           tasks_preInit(void);
void           tasks_init(void);
_Noreturn void tasks_run1Hz(void);
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_runChimera(void);
_Noreturn void tasks_runCan1Tx(void);
_Noreturn void tasks_runCan2Tx(void);
_Noreturn void tasks_runCan3Tx(void);
_Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_batteryMonitoring(void);
_Noreturn void tasks_powerMonitoring(void);