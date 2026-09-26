#pragma once

#include <cassert>
#include <cmsis_os.h>
#include <algorithm>

#if configUSE_TRACE_FACILITY != 1
#error "configUSE_TRACE_FACILITY must be set to 1 in FreeRTOSConfig.h to use runTimeStat"
#endif

#include "hw_hal.hpp"
#include "hw_rtosTaskHandler.hpp"

#ifdef STM32F412Rx
#include "stm32f4xx_hal_tim.h"
#elifdef STM32H562xx
#include "stm32h5xx_hal_tim.h"
#elifdef STM32H733xx
#include "stm32h7xx_hal_tim.h"
#endif

#include "io_log.hpp"

namespace hw::runtimeStat
{
void init(TIM_HandleTypeDef &htim);
void inc();

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

    // IDLE and Tmr Svc are created by the scheduler itself, so the OS always reports TaskCount + 2 tasks.
    // Every board must register every task it starts; tasks_init() checks that at startup.
    static constexpr size_t NUM_FT_TASKS    = 2U;
    static constexpr size_t NUM_TOTAL_TASKS = NUM_FT_TASKS + TaskCount;

    std::array<TaskInfoInternal, TaskCount> _tasks_info;

    CpuInfoBroadcasters _cpu_info;
    struct TaskData
    {
        float max_cpu_usage = 0.0f;
    };
    mutable std::array<TaskData, TaskCount> _tasks_data{};
    mutable float max_cpu_usage = 0.0f;

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
    }

    /**
     * Join between two tables runTimeStats and _tasks_info 
     * runTimeStats: filled by the OS and holds measurements about stack usage and task runtime
     * _tasks_info: routes runTimeStats, which CAN setter each task's number goes to
     */
    void checkin() const
    {
        // 1. Get shi, takes results from the OS and fills it into the runTimeStats array (of type TaskStatus_t)
        std::array<TaskStatus_t, NUM_TOTAL_TASKS> runTimeStats{};
        uint32_t                                    ulTotalRunTime = 0;
        const uint32_t                              arraySize      = uxTaskGetSystemState(
            runTimeStats.data(), static_cast<UBaseType_t>(NUM_TOTAL_TASKS), &ulTotalRunTime);

        if (arraySize == 0)
        {
            // The OS is running more tasks than NUM_TOTAL_TASKS: one was started but never registered
            LOG_ERROR("TaskGetSystemState failed");
            return;
        }
        if (ulTotalRunTime == 0)
        {
            // No profiling time base yet, so every percentage below would be a divide by zero
            LOG_ERROR("Run time counter is 0, is configGENERATE_RUN_TIME_STATS enabled?");
            return;
        }

        /* 2. IDLE Extraction, grabs the idle task's run-time counter
         * Given each task that we get from the following getsystemstate call we are gonna calculate the
         * cpu usage and stack usage
         */
        TaskHandle_t idle_handle = xTaskGetIdleTaskHandle();
        uint32_t idle_index ;
        for (idle_index = 0; idle_index < arraySize; idle_index++)
        {
            if (runTimeStats[idle_index].xHandle == idle_handle) break;
        }
        if (idle_index == arraySize)
        {
            LOG_ERROR("IDLE task not found in taskgetsystemstate results");
            return;
        }
        uint32_t idle_counter = runTimeStats[idle_index].ulRunTimeCounter;

        // 3.  Calculate total current cpu usage and max cpu usage
        const float cpu_usage = (1.0f - static_cast<float>(idle_counter) / static_cast<float>(ulTotalRunTime)) * 100.0f;
        _cpu_info.cpu_usage_setter(cpu_usage);
        max_cpu_usage = std::max(max_cpu_usage, cpu_usage);
        _cpu_info.cpu_usage_max_setter(max_cpu_usage);

        /* 4. Match every registered task to its measurement by handle
         * task: indexes our own list
         * stat_index: indexes the OS table
         */
        for (uint32_t task = 0; task < TaskCount; task++)
        {
            // For each task check to see its handle and see if runTimeStats has the same
            const auto handle = _tasks_info[task].t->id();

            uint32_t stat_index;
            for (stat_index = 0; stat_index < arraySize; stat_index++)
            {
                if (runTimeStats[stat_index].xHandle == handle)
                {
                    break;
                }
            }
            if (stat_index == arraySize)
            {
                LOG_ERROR("Registered task %lu missing from taskgetsystemstate results", task);
                continue;
            }
            TaskStatus_t &status = runTimeStats[stat_index];

            // Calculate current cpu usage
            const float task_cpu_usage =
                static_cast<float>(status.ulRunTimeCounter) / static_cast<float>(ulTotalRunTime) * 100.0f;
            _tasks_info[task].cpu_usage_setter(task_cpu_usage);

            // Calculate the max cpu usage
            _tasks_data[task].max_cpu_usage = std::max(_tasks_data[task].max_cpu_usage, task_cpu_usage);
            _tasks_info[task].cpu_usage_max_setter(_tasks_data[task].max_cpu_usage);

            // Calculate max stack usage
            const float max_stack_usage = (1.0f - static_cast<float>(status.usStackHighWaterMark) /
                                                      static_cast<float>(_tasks_info[task].t->stackSize())) *
                                          100.0f;
            _tasks_info[task].stack_usage_max_setter(max_stack_usage);
        }
    }
};
} // namespace hw::runtimeStat
