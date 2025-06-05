#pragma once

void tasks_preInit();
void tasks_init();

_Noreturn void tasks_runCanFDTx(void);
_Noreturn void tasks_runCanSxTx(void);
_Noreturn void tasks_runCanInvTx(void);


_Noreturn void tasks_runcanRx(void);
_Noreturn void tasks_run10Hz(void);
