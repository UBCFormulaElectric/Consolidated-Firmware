#pragma once

#include "hw_utils.hpp"
#include "util_errorCodes.hpp"

#include <cassert>
#include <cstring>
#include <cmsis_os.h>

#ifdef STM32F412Rx
#include "stm32f4xx_hal_tim.h"
#elif STM32H733xx
#include "stm32h7xx_hal_tim.h"
#endif

extern volatile unsigned long ulHighFrequencyTimerTick;

namespace hw
{
template <size_t TaskCount> class runTimeStat
{
  public:
    struct TaskInfo
    {
        void (*cpu_usage_setter)(float);
        void (*stack_usage_max_setter)(float);
        void (*cpu_usage_max_setter)(float);
    };

    struct CpuInfoBroadcasters
    {
        void (*cpu_usage_setter)(float);
        void (*cpu_usage_max_setter)(float);
    };

  private:
    static constexpr size_t IDLE_TASK_INDEX = 0U;
    static constexpr size_t TMR_SVC_INDEX   = 1U;
    static constexpr size_t NUM_FT_TASKS    = 2U;
    static constexpr size_t NUM_TOTAL_TASKS = NUM_FT_TASKS + TaskCount;

    TIM_HandleTypeDef                    &runTimeCounter;
    std::array<TaskInfo, NUM_TOTAL_TASKS> _tasks_info{};
    CpuInfoBroadcasters                   _cpu_info;
    volatile unsigned long                ulHighFrequencyTimerTick = 0;

    struct TaskData
    {
        float max_cpu_usage = 0.0f;
    };
    std::array<TaskData, NUM_TOTAL_TASKS> _tasks_data{};
    float                                 max_cpu_usage = 0.0f;

  public:
    runTimeStat(TIM_HandleTypeDef &htim, const CpuInfoBroadcasters c, const std::array<TaskInfo, TaskCount> tasks)
      : runTimeCounter(htim), _cpu_info(c)
    {
        // mi bombo
        _tasks_info[IDLE_TASK_INDEX] = {};
        _tasks_info[TMR_SVC_INDEX]   = {};

        assert(c.cpu_usage_max_setter != nullptr);
        assert(c.cpu_usage_setter != nullptr);
        for (size_t i = 0; i < TaskCount; ++i)
        {
            assert(tasks[i].cpu_usage_max_setter != nullptr);
            assert(tasks[i].cpu_usage_setter != nullptr);
            assert(tasks[i].stack_usage_max_setter != nullptr);
            _tasks_info[i + NUM_FT_TASKS] = tasks[i];
        }
    };

    [[nodiscard]] std::expected<void, ErrorCode> init() const
    {
        return utils::convertHalStatus(HAL_TIM_Base_Start_IT(&runTimeCounter));
    }

    /**
     * use this function to update the runtime statistics
     */
    void checkin()
    {
        /* Get the task IDLE handle for processing the time spend outside of idle task*/
        TaskStatus_t   runTimeStats[NUM_TOTAL_TASKS];
        const uint32_t arraySize =
            uxTaskGetSystemState(runTimeStats, static_cast<UBaseType_t>(NUM_TOTAL_TASKS), nullptr);
        if (arraySize == 0)
        {
            LOG_ERROR("TaskGetSystemState failed");
        }

        /*
         * Given each task that we get from the following getsystemstate call we are gonna calculate the
         * cpu usage and stack usage
         */
        uint32_t           idle_counter = 0;
        const TaskHandle_t idleHandle   = xTaskGetIdleTaskHandle();
        // update just the idle task?
        for (uint32_t task = 0; task < arraySize; task++)
        {
            if (runTimeStats[task].xHandle == idleHandle)
            {
                idle_counter = runTimeStats[task].ulRunTimeCounter;

                // Calculate total current cpu usage and max cpu usage
                const float usage =
                    1.0f - static_cast<float>(idle_counter) / static_cast<float>(ulHighFrequencyTimerTick);
                _cpu_info.cpu_usage_setter(usage);
                max_cpu_usage = std::max(max_cpu_usage, usage);
                _cpu_info.cpu_usage_max_setter(max_cpu_usage);
                break;
            }
        }

        for (uint32_t task = 0; task < arraySize; task++)
        {
            // get the idle time that we need to calculate the cpu usage associated
            if (idle_counter + runTimeStats[task].ulRunTimeCounter != 0)
            {
                // Calculate current cpu usage
                const float usage = static_cast<float>(runTimeStats[task].ulRunTimeCounter) /
                                    static_cast<float>(idle_counter + runTimeStats[task].ulRunTimeCounter);
                _tasks_info[task].cpu_usage_setter(usage);

                // Calculate the max cpu usage
                _tasks_data[task].max_cpu_usage = std::max(_tasks_data[task].max_cpu_usage, usage);
                _tasks_info[task].cpu_usage_max_setter(_tasks_data[task].max_cpu_usage);
            }

            // Calculate max stack usage
            _tasks_info[task].stack_usage_max_setter(
                static_cast<float>(runTimeStats[task].usStackHighWaterMark) /
                static_cast<float>(_tasks_info[task]->stack_size));
        }
    }
};
} // namespace hw