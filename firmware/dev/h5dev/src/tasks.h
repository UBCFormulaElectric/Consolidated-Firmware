#pragma once
#include "app_utils.hpp"

CFUNC void       tasks_preInit(void);
CFUNC void       tasks_init(void);
CFUNC NORET void tasks_run1Hz(void);
CFUNC NORET void tasks_run100Hz(void);
CFUNC NORET void tasks_run1kHz(void);
CFUNC NORET void tasks_runCanFDTx(void);
CFUNC NORET void tasks_runCanRx(void);
CFUNC NORET void tasks_runChimera(void);
