#pragma once
#include "util_utils.hpp"

typedef enum
{
    TASK_INDEX_1HZ = 0,
    TASK_INDEX_100HZ,
    TASK_INDEX_1KHZ,
    TASK_INDEX_CANTX,
    TASK_INDEX_CANRX,
    TASK_INDEX_CHIMERA,
    TASK_INDEX_ADBMSVOLTAGES,
    TASK_INDEX_ADBMSTEMPERATURES,
    TASK_INDEX_ADBMSDIAGNOSTICS,
    TASK_INDEX_COUNT
} TaskIndex_e;

CFUNC void       tasks_preInit(void);
CFUNC NORET void tasks_init(void);