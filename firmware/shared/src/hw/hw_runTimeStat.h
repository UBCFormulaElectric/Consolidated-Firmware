#pragma once

#include <stdio.h>
#include "main.h"

typedef struct
{
    float cpu_curr_usage;
    float cpu_max_usage;
    float stack_usage_max;
} RunTimeStats;

void hw_runTimeStat_init(TIM_HandleTypeDef *htim);