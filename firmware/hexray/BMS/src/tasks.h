#pragma once
#include "util_utils.hpp"

CFUNC void       tasks_preInit(void);
CFUNC NORET void tasks_init(void);

// Request the ADBMS config task to run immediately (at its normal priority) instead of
// waiting for its next periodic tick. Safe to call from any task context.
CFUNC void tasks_requestAdbmsConfigRun(void);