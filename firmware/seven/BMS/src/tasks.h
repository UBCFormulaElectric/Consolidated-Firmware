#pragma once
#include "util_utils.hpp"
#include "stm32h7xx_hal_tim.h"

CFUNC void       tasks_handle_arr_rollover_callback(TIM_HandleTypeDef *htim);