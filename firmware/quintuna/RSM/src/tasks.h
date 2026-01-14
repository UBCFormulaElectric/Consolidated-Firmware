#pragma once

#include "io_canMsg.h"

void           tasks_preInit();
void           tasks_init();
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_runCanTx(void);
_Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_run1Hz(void);
void           tasks_runCanRxCallback(const CanMsg *msg);
_Noreturn void tasks_runChimera(void);