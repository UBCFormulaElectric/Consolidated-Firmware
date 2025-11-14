#pragma once

void tasks_preInit();
void tasks_init();

_Noreturn void tasks_runCanFDTx(void);
_Noreturn void tasks_runCanSxTx(void);
_Noreturn void tasks_runCanInvTx(void);

_Noreturn void tasks_runcanRx(void);
_Noreturn void tasks_run1Hz(void);
_Noreturn void tasks_run1kHz(void);
