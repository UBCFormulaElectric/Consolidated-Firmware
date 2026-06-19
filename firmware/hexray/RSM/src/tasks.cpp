#include "tasks.h"
#include "hw_adcs.hpp"
#include "jobs.hpp"

#include "app_jsoncan.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

#include "io_time.hpp"
#include "io_canQueues.hpp"
#include "stm32h5xx_hal.h"
#include "io_canRx.hpp"
#include "io_canTx.hpp"

#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"
#include "main.h"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "hw_pwms.hpp"
#include "hw_bootup.hpp"
#include "hw_runTimeStat.hpp"

constexpr size_t         TASK_COUNT = 5;
[[noreturn]] static void tasks_run1Hz(void *arg);
[[noreturn]] static void tasks_run100Hz(void *arg);
[[noreturn]] static void tasks_run1kHz(void *arg);
[[noreturn]] static void tasks_runImu(void *arg);
[[noreturn]] static void tasks_runCanTx(void *arg);
[[noreturn]] static void tasks_runCanRx(void *arg);

// Define the task with StaticTask Template Class
static hw::rtos::StaticTask::StaticTaskStack<512> Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskImuStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskCanTxStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskCanRxStack;

static hw::rtos::StaticTask Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask TaskImu(osPriorityHigh, "TaskImu", tasks_runImu, TaskImuStack);
static hw::rtos::StaticTask TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx, TaskCanTxStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityLow, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);

static hw::runtimeStat::monitor<TASK_COUNT> runtimeMonitor{
    { app::can_tx::RSM_CoreCpuUsage_set, app::can_tx::RSM_CoreCpuUsageMax_set },
    {
        { { Task1kHz, app::can_tx::RSM_TaskRun1kHzCpuUsage_set, app::can_tx::RSM_TaskRun1kHzCpuUsageMax_set,
            app::can_tx::RSM_TaskRun1kHzStackUsage_set },
          { Task1Hz, app::can_tx::RSM_TaskRun1HzCpuUsage_set, app::can_tx::RSM_TaskRun1HzCpuUsageMax_set,
            app::can_tx::RSM_TaskRun1HzStackUsage_set },
          { Task100Hz, app::can_tx::RSM_TaskRun100HzCpuUsage_set, app::can_tx::RSM_TaskRun100HzCpuUsageMax_set,
            app::can_tx::RSM_TaskRun100HzStackUsage_set },
          { TaskCanRx, app::can_tx::RSM_TaskRunCanRxCpuUsage_set, app::can_tx::RSM_TaskRunCanRxCpuUsageMax_set,
            app::can_tx::RSM_TaskRunCanRxStackUsage_set },
          { TaskCanTx, app::can_tx::RSM_TaskRunCanTxCpuUsage_set, app::can_tx::RSM_TaskRunCanTxCpuUsageMax_set,
            app::can_tx::RSM_TaskRunCanTxStackUsage_set } },
    },
};

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
        io::can_tx::enqueue100HzMsgs();

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
void tasks_runImu(void *arg)
{
    constexpr uint32_t period_ms = 10U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_runImu_tick();

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
            const auto res = can1.fdcan_transmit(hw::CanMsg{
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

static void RSM_StartAllTasks()
{
    Task1kHz.start();
    Task100Hz.start();
    TaskImu.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1Hz.start();
}

void tasks_preInit()
{
    hw::bootup::enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

void tasks_init()
{
#ifndef WATCHDOG_DISABLED
    __HAL_DBGMCU_FREEZE_IWDG();
#endif
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM Reset!");

    osKernelInitialize();

    adcchipsInit();
    can1.init();
    hw::runtimeStat::init(htim7);
    const ResetReason reason = hw::resetReason::get();
    app::can_tx::RSM_ResetReason_set(static_cast<app::can_utils::CanResetReason>(reason));
    if (reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    if (hw::bootup::BootRequest boot_request = hw::bootup::getBootRequest();
        boot_request.context != hw::bootup::BootContext::NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == hw::bootup::BootContext::OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app::can_alerts::infos::StackOverflow_set(true);
            app::can_tx::RSM_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == hw::bootup::BootContext::WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app::can_alerts::infos::WatchdogTimeout_set(true);
            app::can_tx::RSM_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = hw::bootup::BootContext::NONE;
        boot_request.context_value = 0;
        hw::bootup::setBootRequest(boot_request);
    }

    jobs_init();
    RSM_StartAllTasks();
    osKernelStart();
    forever {}
}

void tasks_tim_callback(const TIM_HandleTypeDef *tim)
{
#ifndef USE_CHIMERA
    if (tim == &htim7)
    {
        hw::runtimeStat::inc();
    }
#endif
}
void tasks_handle_arr_rollover_callback(TIM_HandleTypeDef *htim)
{
    if (htim == &flow_meter_config.get_timer_handle())
    {
        flow_meter_config.increment_arrRolloverCount();
    }
}
