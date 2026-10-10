#pragma once

#include <cassert>
#include <algorithm>
#include <array>
#include <cmsis_os.h>

// added the below cuz imma force profiling at runtime for now (may change)
#if configUSE_TRACE_FACILITY != 1
#error "configUSE_TRACE_FACILITY must be set to 1 in FreeRTOSConfig.h to use runTimeStat"
#endif

#if configGENERATE_RUN_TIME_STATS != 1
#error "configGENERATE_RUN_TIME_STATS must be set to 1 in FreeRTOSConfig.h to use runTimeStat"
#endif

#include "hw_hal.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "io_log.hpp"

#if defined(STM32F412Rx)
#include "stm32f4xx_hal_tim.h"
#elif defined(STM32H562xx)
#include "stm32h5xx_hal_tim.h"
#elif defined(STM32H733xx)
#include "stm32h7xx_hal_tim.h"
#endif

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
        const rtos::StaticTask *t             = nullptr; //goood boyy
        void (*cpu_usage_setter)(float)       = nullptr;
        void (*cpu_usage_max_setter)(float)   = nullptr;
        void (*stack_usage_max_setter)(float) = nullptr;
    };

    std::array<TaskInfoInternal, TaskCount> _tasks_info{};
    CpuInfoBroadcasters                     _cpu_info{};

    mutable std::array<uint32_t, TaskCount> _prev_task_runtime{};
    mutable std::array<float, TaskCount>    _max_task_cpu_usage{};
    mutable std::array<float, TaskCount>    _max_stack_usage{};

    mutable uint32_t _prev_total_runtime = 0;
    mutable uint32_t _prev_idle_runtime  = 0;
    mutable float    _max_cpu_usage      = 0.0f;
    mutable bool     _initialized        = false;

  public:
    monitor(const CpuInfoBroadcasters c, const std::array<TaskInfo, TaskCount> tasks) : _cpu_info(c)
    {
        for (size_t i = 0; i < TaskCount; ++i)
        {
            _tasks_info[i] = {
                .t                      = &tasks[i].t,
                .cpu_usage_setter       = tasks[i].cpu_usage_setter,
                .cpu_usage_max_setter   = tasks[i].cpu_usage_max_setter,
                .stack_usage_max_setter = tasks[i].stack_usage_max_setter,
            };
        }
    }

    /**
     * updates CPU and stack usage metrics over the last window
     * does not suspend scheduler dis time
     */
    void checkin() const
    {
        const uint32_t current_total_time = getRunTimeCounterValue();

        // get IDLE task runtime via O(1) TCB read (no memory scan)
        TaskHandle_t idle_handle       = xTaskGetIdleTaskHandle();
        uint32_t     current_idle_time = 0;
        if (idle_handle != nullptr)
        {
            TaskStatus_t idle_status;
            vTaskGetInfo(idle_handle, &idle_status, pdFALSE, eRunning);
            current_idle_time = idle_status.ulRunTimeCounter;
        }

        // on first invocation, establish the baseline to avoid initial distortion
        if (!_initialized)
        {
            _prev_total_runtime = current_total_time;
            _prev_idle_runtime  = current_idle_time;
            for (size_t i = 0; i < TaskCount; ++i)
            {
                if (_tasks_info[i].t == nullptr)
                {
                    continue;
                }
                const auto handle = static_cast<TaskHandle_t>(_tasks_info[i].t->id());
                if (handle != nullptr)
                {
                    TaskStatus_t status;
                    vTaskGetInfo(handle, &status, pdFALSE, eRunning);
                    _prev_task_runtime[i] = status.ulRunTimeCounter;
                }
            }
            _initialized = true;
            return;
        }

        // compute elapsed deltas using unsigned modular arithmetic (rollover safe)
        const uint32_t delta_total = current_total_time - _prev_total_runtime;
        const uint32_t delta_idle  = current_idle_time - _prev_idle_runtime;
        _prev_total_runtime        = current_total_time;
        _prev_idle_runtime         = current_idle_time;

        // calculate Core CPU usage over the window
        if (delta_total > 0)
        {
            const uint32_t clamped_idle = std::min(delta_idle, delta_total);
            const float    cpu_usage =
                (1.0f - static_cast<float>(clamped_idle) / static_cast<float>(delta_total)) * 100.0f;
            const float clamped_cpu_usage = std::clamp(cpu_usage, 0.0f, 100.0f);

            if (_cpu_info.cpu_usage_setter != nullptr)
            {
                _cpu_info.cpu_usage_setter(clamped_cpu_usage);
            }
            _max_cpu_usage = std::max(_max_cpu_usage, clamped_cpu_usage);
            if (_cpu_info.cpu_usage_max_setter != nullptr)
            {
                _cpu_info.cpu_usage_max_setter(_max_cpu_usage);
            }
        }

        // check each registered task individually (preemptible O(1) runtime, preemptible stack check)
        for (size_t i = 0; i < TaskCount; ++i)
        {
            if (_tasks_info[i].t == nullptr)
            {
                continue;
            }

            const auto handle = static_cast<TaskHandle_t>(_tasks_info[i].t->id());
            if (handle == nullptr)
            {
                continue;
            }

            // task cpu usage delta
            TaskStatus_t status;
            vTaskGetInfo(handle, &status, pdFALSE, eRunning);

            const uint32_t delta_task = status.ulRunTimeCounter - _prev_task_runtime[i];
            _prev_task_runtime[i]     = status.ulRunTimeCounter;

            if (delta_total > 0)
            {
                const float task_cpu = (static_cast<float>(delta_task) / static_cast<float>(delta_total)) * 100.0f;
                const float clamped_task_cpu = std::clamp(task_cpu, 0.0f, 100.0f);

                if (_tasks_info[i].cpu_usage_setter != nullptr)
                {
                    _tasks_info[i].cpu_usage_setter(clamped_task_cpu);
                }
                _max_task_cpu_usage[i] = std::max(_max_task_cpu_usage[i], clamped_task_cpu);
                if (_tasks_info[i].cpu_usage_max_setter != nullptr)
                {
                    _tasks_info[i].cpu_usage_max_setter(_max_task_cpu_usage[i]);
                }
            }

            // used preemptible high-water mark query
            const UBaseType_t free_words  = uxTaskGetStackHighWaterMark(handle);
            const size_t      total_words = _tasks_info[i].t->stackSize();
            if (total_words > 0)
            {
                float stack_usage = 0.0f;
                if (free_words < total_words)
                {
                    stack_usage = (1.0f - static_cast<float>(free_words) / static_cast<float>(total_words)) * 100.0f;
                }
                const float clamped_stack_usage = std::clamp(stack_usage, 0.0f, 100.0f);
                _max_stack_usage[i]             = std::max(_max_stack_usage[i], clamped_stack_usage);

                if (_tasks_info[i].stack_usage_max_setter != nullptr)
                {
                    _tasks_info[i].stack_usage_max_setter(_max_stack_usage[i]);
                }
            }
        }
    }
};
} // namespace hw::runtimeStat
