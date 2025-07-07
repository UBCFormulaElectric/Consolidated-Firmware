#pragma once

#include <stdio.h>
#include "main.h"

typedef struct
{
    float cpu_usage;
    float stack_usage;
} RunTimeStats;

void hw_runTimeStat_init(TIM_HandleTypeDef *htim);