#include "tasks.h"
#include "jobs.hpp"
#include "main.h"

#include "app_jsoncan.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"

#include "io_canQueues.hpp"
#include "io_time.hpp"
#include "io_canRx.hpp"

#include "hw_adcs.hpp"
#include "hw_watchdog.hpp"
#include "hw_cans.hpp"
#include "hw_error.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_resetReason.hpp"
#include "hw_bootup.hpp"
#include "hw_pwms.hpp"
// #include "hw_runTimeStat.hpp"

constexpr size_t         TASK_COUNT = 8;
[[noreturn]] static void tasks_run1Hz(void *arg);
[[noreturn]] static void tasks_run100Hz(void *arg);
[[noreturn]] static void tasks_run1kHz(void *arg);
[[noreturn]] static void tasks_runCanTx(void *arg);
[[noreturn]] static void tasks_runCanRx(void *arg);
[[noreturn]] static void tasks_runAdbmsVoltages(void *arg);
[[noreturn]] static void tasks_runAdbmsConfigs(void *arg);
[[noreturn]] static void tasks_runAdbmsAux(void *arg);

static hw::rtos::StaticTask::StaticTaskStack<512>      Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>      Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<1024 * 6> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>      TaskCanRxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>      TaskCanTxStack;
static hw::rtos::StaticTask::StaticTaskStack<1024 * 3> TaskAdbmsVoltagesStack;
static hw::rtos::StaticTask::StaticTaskStack<1024 * 3> TaskAdbmsConfigsStack;
static hw::rtos::StaticTask::StaticTaskStack<1024 * 3> TaskAdbmsAuxStack;

static hw::rtos::StaticTask Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityBelowNormal, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);
static hw::rtos::StaticTask TaskCanTx(osPriorityBelowNormal, "TaskCanTx", tasks_runCanTx, TaskCanTxStack);
static hw::rtos::StaticTask
    TaskAdbmsVoltages(osPriorityNormal, "TaskAdbmsVoltages", tasks_runAdbmsVoltages, TaskAdbmsVoltagesStack);
static hw::rtos::StaticTask
    TaskAdbmsConfigs(osPriorityHigh, "TaskAdbmsConfigs", tasks_runAdbmsConfigs, TaskAdbmsConfigsStack);
static hw::rtos::StaticTask TaskAdbmsAux(osPriorityNormal, "TaskAdbmsAux", tasks_runAdbmsAux, TaskAdbmsAuxStack);

// static hw::runtimeStat::monitor<TASK_COUNT> runtimeMonitor(
//     {
//         app::can_tx::BMS_CoreCpuUsage_set,
//         app::can_tx::BMS_CoreCpuUsageMax_set,
//     },
//     { {
//         {
//             Task1kHz,
//             app::can_tx::BMS_TaskRun1kHzCpuUsage_set,
//             app::can_tx::BMS_TaskRun1kHzCpuUsageMax_set,
//             app::can_tx::BMS_TaskRun1kHzStackUsage_set,
//         },
//         {
//             Task1Hz,
//             app::can_tx::BMS_TaskRun1HzCpuUsage_set,
//             app::can_tx::BMS_TaskRun1HzCpuUsageMax_set,
//             app::can_tx::BMS_TaskRun1HzStackUsage_set,
//         },
//         {
//             Task100Hz,
//             app::can_tx::BMS_TaskRun100HzCpuUsage_set,
//             app::can_tx::BMS_TaskRun100HzCpuUsageMax_set,
//             app::can_tx::BMS_TaskRun100HzStackUsage_set,
//         },
//         {
//             TaskCanRx,
//             app::can_tx::BMS_TaskRunCanRxCpuUsage_set,
//             app::can_tx::BMS_TaskRunCanRxCpuUsageMax_set,
//             app::can_tx::BMS_TaskRunCanRxStackUsage_set,
//         },
//         {
//             TaskCanTx,
//             app::can_tx::BMS_TaskRunCanTxCpuUsage_set,
//             app::can_tx::BMS_TaskRunCanTxCpuUsageMax_set,
//             app::can_tx::BMS_TaskRunCanTxStackUsage_set,
//         },
//         {
//             TaskAdbmsVoltages,
//             app::can_tx::BMS_TaskRunAdbmsVoltagesCpuUsage_set,
//             app::can_tx::BMS_TaskRunAdbmsVoltagesCpuUsageMax_set,
//             app::can_tx::BMS_TaskRunAdbmsVoltagesStackUsage_set,
//         },
//         {
//             TaskAdbmsConfigs,
//             app::can_tx::BMS_TaskRunAdbmsConfigsCpuUsage_set,
//             app::can_tx::BMS_TaskRunAdbmsConfigsCpuUsageMax_set,
//             app::can_tx::BMS_TaskRunAdbmsConfigsStackUsage_set,
//         },
//         {
//             TaskAdbmsAux,
//             app::can_tx::BMS_TaskRunAdbmsAuxCpuUsage_set,
//             app::can_tx::BMS_TaskRunAdbmsAuxCpuUsageMax_set,
//             app::can_tx::BMS_TaskRunAdbmsAuxStackUsage_set,
//         },
//     } });

static hw::watchdog::monitor<TASK_COUNT> monitor{
    hiwdg1,
    [](const hw::watchdog::instance &i) { LOG_ERROR("Watchdog timeout detected in task %d", i.task_id); },
};

void tasks_run1Hz(void *arg)
{
    constexpr uint32_t      period_ms                = 1000U;
    constexpr uint32_t      watchdog_grace_period_ms = 50U;
    hw::watchdog::instance &watchdog1hz              = monitor.spawn_instance(period_ms + watchdog_grace_period_ms);
    uint32_t                start_ticks              = osKernelGetTickCount();

    forever
    {
        // SEGGER_SYSVIEW_MarkStart(100);
        jobs_run1Hz_tick();
        // SEGGER_SYSVIEW_MarkStop(100);
        // SEGGER_SYSVIEW_MarkStart(101);
        watchdog1hz.checkIn();
        // SEGGER_SYSVIEW_MarkStop(101);
        // SEGGER_SYSVIEW_MarkStart(102);
        // runtimeMonitor.checkin();
        // SEGGER_SYSVIEW_MarkStop(102);
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
    uint32_t                start_ticks              = osKernelGetTickCount();

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
        // Elcon only supports regular CAN but we have some debug messages that are >8 bytes long. Use FDCAN for those
        // (they won't get seen by the charger, but they'll show up on CANoe).
        // TODO: Bit-rate-switching wasn't working for me when the BMS was connected to the charger, so the FD
        // peripheral is configured without BRS. Figure out why it wasn't working?

        const auto msg = can_tx_queue.pop();
        if (not msg)
            continue;

        const auto &m       = msg.value();
        auto        can_msg = hw::CanMsg{ m.std_id, m.dlc, m.data };

        if (m.bus == app::can_utils::BusEnum::Bus_charger)
        {
            result<void> res;
            if (can_msg.dlc > 8)
            {
                res = fdcan1.fdcan_transmit(can_msg);
            }
            else
            {
                res = fdcan1.can_transmit(can_msg);
            }
            LOG_IF_ERR(res);
        }
        else if (m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = fdcan2.fdcan_transmit(can_msg);
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

void tasks_runAdbmsVoltages(void *arg)
{
    const uint32_t period_ms   = 500U;
    uint32_t       start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_runAdbmsVoltages_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runAdbmsConfigs(void *arg)
{
    const uint32_t period_ms   = 200U;
    uint32_t       start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_runAdbmsConfigs_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runAdbmsAux(void *arg)
{
    const uint32_t period_ms   = 500U;
    uint32_t       start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_runAdbmsAux_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void BMS_StartAllTasks()
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskAdbmsVoltages.start();
    TaskAdbmsConfigs.start();
    TaskAdbmsAux.start();
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
    LOG_INFO("BMS Reset!");

#ifndef WATCHDOG_DISABLED
    __HAL_DBGMCU_FREEZE_IWDG1();
#endif

    adcChipsInit();
    pwms_init();
    fdcan1.init();
    fdcan2.init();

    const ResetReason reset_reason = hw::resetReason::get();
    app::can_tx::BMS_ResetReason_set(static_cast<app::can_utils::CanResetReason>(reset_reason));

    if (reset_reason == ResetReason::RESET_REASON_WATCHDOG)
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
            app::can_tx::BMS_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == hw::bootup::BootContext::WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app::can_alerts::infos::WatchdogTimeout_set(true);
            app::can_tx::BMS_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = hw::bootup::BootContext::NONE;
        boot_request.context_value = 0;
        hw::bootup::setBootRequest(boot_request);
    }

    jobs_init();
    osKernelInitialize();
    BMS_StartAllTasks();
    osKernelStart();
    Error_Handler();
}
