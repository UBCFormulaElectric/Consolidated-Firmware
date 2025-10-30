#pragma once

#include "io_canMsg.h"

void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run1Hz(void *arg);
_Noreturn void tasks_run100Hz(void *arg);
_Noreturn void tasks_run1kHz(void *arg);
_Noreturn void tasks_runCanTx(void *arg);
void           tasks_runCanRxCallback(const CanMsg *msg);
_Noreturn void tasks_runCanRx(void *arg);
_Noreturn void tasks_runChimera(void *arg);