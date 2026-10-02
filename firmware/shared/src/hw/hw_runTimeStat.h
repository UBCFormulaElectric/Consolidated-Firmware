#pragma once

#include <stdio.h>
#include "main.h"
#include "util_errorCodes.h"

#ifdef STM32F412Rx
#include "stm32f4xx_hal_tim.h"
#elif STM32H733xx
#include "stm32h7xx_hal_tim.h"
#endif

extern volatile unsigned long ulHighFrequencyTimerTick;

// Task instance for runtimestats
typedef struct
{
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

    /*
     * Stack Size
     */
    uint16_t stack_size;

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

typedef struct
{
    /*
     * CPU usage
     */
    float cpu_curr_usage;

    /*
     * Max CPU usage
     */
    float cpu_max_usage;

    // Setter function pointers
    /*
     * CPU usage can setter function
     */
    void (*cpu_usage_setter)(float);

    /*
     * Max CPU usage can setter function
     */
    void (*cpu_usage_max_setter)(float);
} CpuRunTimeStats;

ExitCode hw_runTimeStat_init(TIM_HandleTypeDef *htim);

void hw_runTimeStat_hookCallBack(void);

ExitCode hw_runTimeStat_registerTask(TaskRuntimeStats *task_info);

ExitCode hw_runtimeStat_registerCpu(CpuRunTimeStats *cpu_info);
