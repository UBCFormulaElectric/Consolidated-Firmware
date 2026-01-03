#pragma once

void           tasks_preInitWatchdog(void);
void           tasks_preInit(void);
void           tasks_init(void);
_Noreturn void tasks_run1Hz(void);
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_runChimera(void);
// _Noreturn void tasks_runCan1Tx(void);
// _Noreturn void tasks_runCan2Tx(void);
// _Noreturn void tasks_runCan3Tx(void);
// _Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_batteryMonitoring(void);
_Noreturn void tasks_powerMonitoring(void);