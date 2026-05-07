#pragma once
#include "util_utils.hpp"
#include "stm32h5xx_hal_tim.h"

CFUNC void       tasks_preInit(void);
CFUNC NORET void tasks_init(void);
CFUNC void       tasks_tim_callback(const TIM_HandleTypeDef *htim);