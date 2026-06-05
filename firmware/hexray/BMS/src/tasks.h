#pragma once
#include "util_utils.hpp"
#include "hw_hal.hpp"
#include "stm32h7xx_hal_tim.h"

typedef enum
{
    TASK_INDEX_1HZ = 0,
    TASK_INDEX_100HZ,
    TASK_INDEX_1KHZ,
    TASK_INDEX_CANTX,
    TASK_INDEX_CANRX,
    TASK_INDEX_CHIMERA,
    TASK_INDEX_COUNT
} TaskIndex_e;

CFUNC void       tasks_preInit(void);
CFUNC NORET void tasks_init(void);
CFUNC void       tasks_handle_arr_rollover_callback(TIM_HandleTypeDef *htim);
