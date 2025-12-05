#pragma once
#include "util_utils.hpp"
// #include "io_canMsg.h"

void             tasks_preInit();
void             tasks_init();
CFUNC NORET void tasks_run1Hz();
CFUNC NORET void tasks_run100Hz();
CFUNC NORET void tasks_run1kHz();
CFUNC NORET void tasks_runCanTx();
// void           tasks_runCanRxCallback(const CanMsg *msg);
CFUNC NORET void tasks_runCanRx();
CFUNC NORET void tasks_runChimera(void);