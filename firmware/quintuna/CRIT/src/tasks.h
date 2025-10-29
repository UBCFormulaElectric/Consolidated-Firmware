#pragma once

void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run1Hz(void *arg);
_Noreturn void tasks_run100Hz(void *arg);
_Noreturn void tasks_run1kHz(void *arg);
_Noreturn void tasks_runCanTx(void *arg);
_Noreturn void tasks_runCanRx(void *arg);