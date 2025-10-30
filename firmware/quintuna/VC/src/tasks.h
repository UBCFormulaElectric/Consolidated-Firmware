#pragma once

void           tasks_preInitWatchdog(void);
void           tasks_preInit(void);
void           tasks_init(void);
_Noreturn void tasks_run1Hz(void *arg);
_Noreturn void tasks_run100Hz(void *arg);
_Noreturn void tasks_run1kHz(void *arg);
_Noreturn void tasks_runChimera(void *arg);
_Noreturn void tasks_runCan1Tx(void *arg);
_Noreturn void tasks_runCan2Tx(void *arg);
_Noreturn void tasks_runCan3Tx(void *arg);
_Noreturn void tasks_runCanRx(void *arg);
_Noreturn void tasks_batteryMonitoring(void *arg);
_Noreturn void tasks_powerMonitoring(void *arg);