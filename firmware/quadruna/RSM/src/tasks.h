#pragma once

void           tasks_deinit(void);
void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_runCanTx(void);
_Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_run1Hz(void);
