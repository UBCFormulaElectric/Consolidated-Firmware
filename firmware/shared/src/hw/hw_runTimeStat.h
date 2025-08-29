#pragma once

#include <stdio.h>
#include "main.h"

#define MAX_TASKS 10

extern volatile unsigned long ulHighFrequencyTimerTick;

typedef struct
{
    float cpu_curr_usage;
    float cpu_max_usage;
    float stack_usage_max;
} RunTimeStats;

typedef struct
{
    void (*cpu_usage_setter)(float);
    void (*stack_usage_max_setter)(float);
    void (*cpu_usage_max_setter)(float);
} RunTimeStatsPublish;


void hw_runTimeStat_init(TIM_HandleTypeDef *htim, const RunTimeStatsPublish *const task_publish_info);


void hw_runTimeStat_hookCallBack(void);
