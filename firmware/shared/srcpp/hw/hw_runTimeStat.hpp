#pragma once

#include "util_errorCodes.hpp"

#ifdef STM32F412Rx
#include "stm32f4xx_hal_tim.h"
#elif STM32H733xx
#include "stm32h7xx_hal_tim.h"
#endif

extern volatile unsigned long ulHighFrequencyTimerTick;

namespace hw::runtimeStat
{
// Task instance for runtimestats
struct TaskRuntimeStats
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
};

struct CpuRunTimeStats
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
};

[[nodiscard]] std::expected<void, ErrorCode> init(TIM_HandleTypeDef &htim);
[[nodiscard]] std::expected<void, ErrorCode> registerTask(TaskRuntimeStats &task_info);
[[nodiscard]] std::expected<void, ErrorCode> registerCpu(CpuRunTimeStats &cpu_info);

void hookCallBack();
} // namespace hw::runtimeStat