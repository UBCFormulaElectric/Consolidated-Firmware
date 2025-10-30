#pragma once

void           tasks_preInitWatchdog(void);
void           tasks_preInit(void);
void           tasks_init(void);
_Noreturn void tasks_run1Hz(void *arg);
_Noreturn void tasks_run100Hz(void *arg);
_Noreturn void tasks_run1kHz(void *arg);
_Noreturn void tasks_runCanTx(void *arg);
_Noreturn void tasks_runCanRx(void *arg);
_Noreturn void tasks_runTelem(void *arg);
_Noreturn void tasks_runTelemRx(void *arg);
_Noreturn void tasks_runLogging(void *arg);
_Noreturn void tasks_runChimera(void *arg);
