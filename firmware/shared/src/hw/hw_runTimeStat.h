#pragma once

#include <stdio.h>
#include "main.h"
#include "app_utils.h"

extern volatile unsigned long ulHighFrequencyTimerTick;

// Task instance for runtimestats
typedef struct
{
    // Info regarding the runtimestats

    /*
     * CPU usage
     */
    float cpu_curr_usage;

    /*
     * Max CPU usage
     */
    float cpu_max_usage;

    /*
     * Max stack usage
     */
    float stack_usage_max;

    /*
     * Task Index
     */
    uint16_t task_index;

    // Setter function pointers

    /*
     * CPU usage can setter function
     */
    void (*cpu_usage_setter)(float);

    /*
     * Max stack usage can setter function
     */
    void (*stack_usage_max_setter)(float);

    /*
     * Max CPU usage can setter function
     */
    void (*cpu_usage_max_setter)(float);

} TaskRuntimeStats;

ExitCode hw_runTimeStat_init(TIM_HandleTypeDef *htim);

void hw_runTimeStat_hookCallBack(void);
