#pragma once
#include "util_utils.hpp"
#include "hw_hal.hpp"
#include "stm32h7xx_hal_tim.h"

CFUNC void       tasks_preInit(void);
CFUNC NORET void tasks_init(void);
CFUNC void       tasks_requestAdbmsConfigRun(void);
CFUNC void       tasks_handle_arr_rollover_callback(TIM_HandleTypeDef *htim);
