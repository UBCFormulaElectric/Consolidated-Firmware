#pragma once
#include "util_utils.hpp"

CFUNC void       tasks_preInit(void);
CFUNC void       tasks_init(void);
CFUNC NORET void tasks_run1Hz(void *arg);
CFUNC NORET void tasks_run100Hz(void *arg);
CFUNC NORET void tasks_run1kHz(void *arg);
CFUNC NORET void tasks_runChimera(void *arg);
