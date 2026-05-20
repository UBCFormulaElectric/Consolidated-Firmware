#include "tasks.h"
#include "jobs.hpp"
#include "main.h"

#include "app_jsoncan.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

#include "io_time.hpp"
#include "io_canRx.hpp"
#include "io_canQueues.hpp"

#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "hw_bootup.hpp"
#include "hw_runTimeStat.hpp"

static constexpr size_t  TASK_COUNT = 5;
[[noreturn]] static void tasks_run1Hz(void *arg);
[[noreturn]] static void tasks_run100Hz(void *arg);
[[noreturn]] static void tasks_run1kHz(void *arg);
[[noreturn]] static void tasks_runCanTx(void *arg);
[[noreturn]] static void tasks_runCanRx(void *arg);

// Define the task with StaticTask template class
static hw::rtos::StaticTask::StaticTaskStack<512> Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskCanRxStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskCanTxStack;

static hw::rtos::StaticTask Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);
static hw::rtos::StaticTask TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx, TaskCanTxStack);

static hw::runtimeStat::monitor<TASK_COUNT> runtimeMonitor(
    {
        app::can_tx::CRIT_CoreCpuUsage_set,
        app::can_tx::CRIT_CoreCpuUsageMax_set,
    },
    { {
        {
            Task1kHz,
            app::can_tx::CRIT_TaskRun1kHzCpuUsage_set,
            app::can_tx::CRIT_TaskRun1kHzCpuUsageMax_set,
            app::can_tx::CRIT_TaskRun1kHzStackUsage_set,
        },
        {
            Task1Hz,
            app::can_tx::CRIT_TaskRun1HzCpuUsage_set,
            app::can_tx::CRIT_TaskRun1HzCpuUsageMax_set,
            app::can_tx::CRIT_TaskRun1HzStackUsage_set,
        },
        {
            Task100Hz,
            app::can_tx::CRIT_TaskRun100HzCpuUsage_set,
            app::can_tx::CRIT_TaskRun100HzCpuUsageMax_set,
            app::can_tx::CRIT_TaskRun100HzStackUsage_set,
        },
        {
            TaskCanRx,
            app::can_tx::CRIT_TaskRunCanRxCpuUsage_set,
            app::can_tx::CRIT_TaskRunCanRxCpuUsageMax_set,
            app::can_tx::CRIT_TaskRunCanRxStackUsage_set,
        },
        {
            TaskCanTx,
            app::can_tx::CRIT_TaskRunCanTxCpuUsage_set,
            app::can_tx::CRIT_TaskRunCanTxCpuUsageMax_set,
            app::can_tx::CRIT_TaskRunCanTxStackUsage_set,
        },
    } });

static hw::watchdog::monitor<TASK_COUNT> monitor{
    hiwdg,
    [](const hw::watchdog::instance &i) { LOG_INFO("Software watchdog timeout for task %d", i.task_id); },
};

void tasks_run1Hz(void *arg)
{
    constexpr uint32_t      period_ms                = 1000U;
    constexpr uint32_t      watchdog_grace_period_ms = 50U;
    hw::watchdog::instance &watchdog1hz              = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1Hz_tick();
        runtimeMonitor.checkin();

        watchdog1hz.checkIn();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}
void tasks_run100Hz(void *arg)
{
    constexpr uint32_t      period_ms                = 10U;
    constexpr uint32_t      watchdog_grace_period_ms = 2U;
    hw::watchdog::instance &watchdog100hz            = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run100Hz_tick();

        watchdog100hz.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
void tasks_run1kHz(void *arg)
{
    constexpr uint32_t      period_ms                = 1U;
    constexpr uint32_t      watchdog_grace_period_ms = 1U;
    hw::watchdog::instance &watchdog1khz             = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1kHz_tick();

        watchdog1khz.checkIn();
        monitor.checkForTimeouts();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
void tasks_runCanTx(void *arg)
{
    forever
    {
        const auto msg = can_tx_queue.pop();
        if (not msg)
            continue;
        if (const auto &m = msg.value(); m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = fdcan1.fdcan_transmit(hw::CanMsg{
                m.std_id,
                m.dlc,
                m.data,
            });
            LOG_IF_ERR(res);
        }
        else
        {
            LOG_ERROR("INVALID BUS %d", m.bus);
        }
    }
}
void tasks_runCanRx(void *arg)
{
    forever
    {
        const auto msg = can_rx_queue.pop();
        if (not msg)
            continue;
        io::can_rx::updateRxTableWithMessage(app::jsoncan::copyFromCanMsg(msg.value()));
    }
}

static void CRIT_StartAllTasks()
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
}

void tasks_preInit()
{
#ifdef BOOTLOAD
    hw::bootup::enableInterruptsForApp();
#endif
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();

#ifndef WATCHDOG_DISABLED
    __HAL_DBGMCU_FREEZE_IWDG();
#endif

    fdcan1.init();
    hw::runtimeStat::init(htim7);

    const ResetReason reason = hw::resetReason::get();
    app::can_tx::CRIT_ResetReason_set(static_cast<app::can_utils::CanResetReason>(reason));
    if (reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    if (const hw::bootup::BootRequest boot_request = hw::bootup::getBootRequest();
        boot_request.context != hw::bootup::BootContext::BOOT_CONTEXT_NONE)
    {
        if (boot_request.context == hw::bootup::BootContext::BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app::can_alerts::infos::StackOverflow_set(true);
            app::can_tx::CRIT_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == hw::bootup::BootContext::BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
            app::can_alerts::infos::WatchdogTimeout_set(true);
            app::can_tx::CRIT_Info_WatchdogTimeout_set(boot_request.context_value);
        }
    }

    jobs_init();
    osKernelInitialize();
    CRIT_StartAllTasks();
    osKernelStart();
    forever {}
}

void tasks_tim_callback(const TIM_HandleTypeDef *htim)
{
    if (htim == &htim7)
    {
        hw::runtimeStat::inc();
    }
}
