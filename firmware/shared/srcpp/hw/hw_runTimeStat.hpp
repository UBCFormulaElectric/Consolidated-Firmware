#pragma once

#include <cassert>
#include <cstring>
#include <cmsis_os.h>
#include <algorithm>

#if configUSE_TRACE_FACILITY != 1
#error "configUSE_TRACE_FACILITY must be set to 1 in FreeRTOSConfig.h to use runTimeStat"
#endif

#include "hw_hal.hpp"
#ifdef STM32F412Rx
#include "stm32f4xx_hal_tim.h"
#elifdef STM32H562xx
#include "stm32h5xx_hal_tim.h"
#elifdef STM32H733xx
#endif

#include "hw_rtosTaskHandler.hpp"
#include "io_log.hpp"

namespace hw::runtimeStat
{
void init(TIM_HandleTypeDef &htim);

template <size_t TaskCount> class monitor
{
  public:
    struct TaskInfo
    {
        const rtos::StaticTask &t;
        void (*cpu_usage_setter)(float);
        void (*cpu_usage_max_setter)(float);
        void (*stack_usage_max_setter)(float);
    };

    struct CpuInfoBroadcasters
    {
        void (*cpu_usage_setter)(float);
        void (*cpu_usage_max_setter)(float);
    };

  private:
    struct TaskInfoInternal
    {
        const rtos::StaticTask *t = nullptr; // ugh
        void (*cpu_usage_setter)(float);
        void (*cpu_usage_max_setter)(float);
        void (*stack_usage_max_setter)(float);
    };
    static constexpr size_t NUM_FT_TASKS    = 2U;
    static constexpr size_t NUM_TOTAL_TASKS = NUM_FT_TASKS + TaskCount;
    static constexpr size_t IDLE_TASK_INDEX = NUM_TOTAL_TASKS - 2;
    static constexpr size_t TMR_SVC_INDEX   = NUM_TOTAL_TASKS - 1;

    std::array<TaskInfoInternal, NUM_TOTAL_TASKS> _tasks_info;
    CpuInfoBroadcasters                           _cpu_info;

    struct TaskData
    {
        float max_cpu_usage = 0.0f;
    };
    mutable std::array<TaskData, NUM_TOTAL_TASKS> _tasks_data{};
    mutable float                                 max_cpu_usage = 0.0f;

  public:
    monitor(const CpuInfoBroadcasters c, const std::array<TaskInfo, TaskCount> tasks) : _cpu_info(c)
    {
        assert(c.cpu_usage_max_setter != nullptr);
        assert(c.cpu_usage_setter != nullptr);

        // set the tasks properly
        for (size_t i = 0; i < TaskCount; ++i)
        {
            assert(tasks[i].cpu_usage_max_setter != nullptr);
            assert(tasks[i].cpu_usage_setter != nullptr);
            assert(tasks[i].stack_usage_max_setter != nullptr);
            _tasks_info[i] = {
                .t                      = &tasks[i].t,
                .cpu_usage_setter       = tasks[i].cpu_usage_setter,
                .cpu_usage_max_setter   = tasks[i].cpu_usage_max_setter,
                .stack_usage_max_setter = tasks[i].stack_usage_max_setter,
            };
        }

        // mi bombo
        _tasks_info[IDLE_TASK_INDEX] = {
            .cpu_usage_setter       = nullptr,
            .cpu_usage_max_setter   = nullptr,
            .stack_usage_max_setter = nullptr,
        };
        _tasks_info[TMR_SVC_INDEX] = {
            .cpu_usage_setter       = nullptr,
            .cpu_usage_max_setter   = nullptr,
            .stack_usage_max_setter = nullptr,
        };
    }

    /**
     * use this function to update the runtime statistics
     */
    void checkin() const
    {
        /* Get the task IDLE handle for processing the time spend outside of idle task*/
        std::array<TaskStatus_t, NUM_TOTAL_TASKS> runTimeStats{};
        uint32_t                                  ulTotalRunTime;
        const uint32_t                            arraySize =
            uxTaskGetSystemState(runTimeStats.data(), static_cast<UBaseType_t>(NUM_TOTAL_TASKS), &ulTotalRunTime);
        if (arraySize == 0)
        {
            LOG_ERROR("TaskGetSystemState failed");
            return;
        }
        if (arraySize != NUM_TOTAL_TASKS)
        {
            LOG_ERROR("TaskGetSystemState returned unexpected number of tasks: %lu", arraySize);
            return;
        }

        std::sort(
            runTimeStats.begin(), runTimeStats.end(),
            [](const TaskStatus_t &a, const TaskStatus_t &b) { return a.xTaskNumber < b.xTaskNumber; });

        /*
         * Given each task that we get from the following getsystemstate call we are gonna calculate the
         * cpu usage and stack usage
         */
        const TaskStatus_t &idleTaskStatus = runTimeStats[IDLE_TASK_INDEX];
        assert(idleTaskStatus.xTaskNumber == IDLE_TASK_INDEX + 1);
        assert(strcmp(idleTaskStatus.pcTaskName, "IDLE") == 0);
        uint32_t idle_counter = idleTaskStatus.ulRunTimeCounter;
        // Calculate total current cpu usage and max cpu usage
        const float cpu_usage = (1.0f - static_cast<float>(idle_counter) / static_cast<float>(ulTotalRunTime)) * 100;
        _cpu_info.cpu_usage_setter(cpu_usage);
        max_cpu_usage = std::max(max_cpu_usage, cpu_usage);
        _cpu_info.cpu_usage_max_setter(max_cpu_usage);

        for (uint32_t task = 0; task < NUM_TOTAL_TASKS - NUM_FT_TASKS; task++)
        {
            // get the idle time that we need to calculate the cpu usage associated
            if (idle_counter + runTimeStats[task].ulRunTimeCounter != 0)
            {
                // Calculate current cpu usage
                const float task_cpu_usage = static_cast<float>(runTimeStats[task].ulRunTimeCounter) /
                                             static_cast<float>(idle_counter + runTimeStats[task].ulRunTimeCounter) *
                                             100;
                _tasks_info[task].cpu_usage_setter(task_cpu_usage);

                // Calculate the max cpu usage
                _tasks_data[task].max_cpu_usage = std::max(_tasks_data[task].max_cpu_usage, task_cpu_usage);
                _tasks_info[task].cpu_usage_max_setter(_tasks_data[task].max_cpu_usage);
            }

            // Calculate max stack usage
            const float max_stack_usage = static_cast<float>(runTimeStats[task].usStackHighWaterMark) /
                                          static_cast<float>(_tasks_info[task].t->stackSize()) * 100;
            _tasks_info[task].stack_usage_max_setter(max_stack_usage);
        }
    }
};
} // namespace hw::runtimeStat
