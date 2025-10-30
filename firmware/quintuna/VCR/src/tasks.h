#pragma once

void tasks_preInit();
void tasks_init();

_Noreturn void tasks_runCanFDTx(void *arg);
_Noreturn void tasks_runCanSxTx(void *arg);
_Noreturn void tasks_runCanInvTx(void *arg);

_Noreturn void tasks_runcanRx(void *arg);
_Noreturn void tasks_run1Hz(void *arg);
_Noreturn void tasks_run1kHz(void *arg);
