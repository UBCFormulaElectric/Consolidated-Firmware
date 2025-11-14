#pragma once

void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run1Hz();
_Noreturn void tasks_run100Hz();
_Noreturn void tasks_run1kHz();
_Noreturn void tasks_runCanTx();
_Noreturn void tasks_runCanRx();
_Noreturn void tasks_runChimera(void);