#include "tasks.h"
#include "jobs.hpp"
#include "main.h"

#include "app_jsoncan.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

#include "io_canRx.hpp"
#include "io_canQueues.hpp"
#include "io_time.hpp"

#include "hw_bootup.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "hw_adcs.hpp"
#include "hw_runTimeStat.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg);
[[noreturn]] static void tasks_run100Hz(void *arg);
[[noreturn]] static void tasks_run1kHz(void *arg);
[[noreturn]] static void tasks_runCanTx(void *arg);
[[noreturn]] static void tasks_runCanRx(void *arg);

// Define the task with StaticTask template class
static hw::rtos::StaticTask::StaticTaskStack<512> Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskCanTxStack;
static hw::rtos::StaticTask::StaticTaskStack<512> TaskCanRxStack;

static hw::rtos::StaticTask Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx, TaskCanTxStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);

static const hw::runtimeStat::monitor<5> task_monitor{
    { app::can_tx::FSM_CoreCpuUsage_set, app::can_tx::FSM_CoreCpuUsageMax_set },
    { { {
            Task1kHz,
            app::can_tx::FSM_TaskRun1kHzCpuUsage_set,
            app::can_tx::FSM_TaskRun1kHzCpuUsageMax_set,
            app::can_tx::FSM_TaskRun1kHzStackUsage_set,
        },
        {
            Task100Hz,
            app::can_tx::FSM_TaskRun100HzCpuUsage_set,
            app::can_tx::FSM_TaskRun100HzCpuUsageMax_set,
            app::can_tx::FSM_TaskRun100HzStackUsage_set,
        },
        {
            Task1Hz,
            app::can_tx::FSM_TaskRun1HzCpuUsage_set,
            app::can_tx::FSM_TaskRun1HzCpuUsageMax_set,
            app::can_tx::FSM_TaskRun1HzStackUsage_set,
        },
        {
            TaskCanTx,
            app::can_tx::FSM_TaskRunCanTxCpuUsage_set,
            app::can_tx::FSM_TaskRunCanTxCpuUsageMax_set,
            app::can_tx::FSM_TaskRunCanTxStackUsage_set,
        },
        {
            TaskCanRx,
            app::can_tx::FSM_TaskRunCanRxCpuUsage_set,
            app::can_tx::FSM_TaskRunCanRxCpuUsageMax_set,
            app::can_tx::FSM_TaskRunCanRxStackUsage_set,
        } } },
};

void tasks_run1Hz(void *arg)
{
    constexpr uint32_t             period_ms                = 1000U;
    constexpr uint32_t             watchdog_grace_period_ms = 50U;
    hw::watchdog::WatchdogInstance watchdog1hz{ period_ms + watchdog_grace_period_ms };
    hw::watchdog::monitor          monitor1hz{ &watchdog1hz, hiwdg, HAL_IWDG_Refresh };
    monitor1hz.registerWatchdogInstance();

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1Hz_tick();

        watchdog1hz.checkIn();
        task_monitor.checkin();

        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}
void tasks_run100Hz(void *arg)
{
    constexpr uint32_t             period_ms                = 10U;
    constexpr uint32_t             watchdog_grace_period_ms = 2U;
    hw::watchdog::WatchdogInstance watchdog100hz{ period_ms + watchdog_grace_period_ms };
    hw::watchdog::monitor          monitor100hz{ &watchdog100hz, hiwdg, HAL_IWDG_Refresh };
    monitor100hz.registerWatchdogInstance();

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        float voltage = susp_fl.getVoltage();
        jobs_run100Hz_tick();

        watchdog100hz.checkIn();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
void tasks_run1kHz(void *arg)
{
    constexpr uint32_t             period_ms                = 1U;
    constexpr uint32_t             watchdog_grace_period_ms = 1U;
    hw::watchdog::WatchdogInstance watchdog1khz{ period_ms + watchdog_grace_period_ms };
    hw::watchdog::monitor          monitor1khz{ &watchdog1khz, hiwdg, HAL_IWDG_Refresh };
    monitor1khz.registerWatchdogInstance();

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1kHz_tick();

        watchdog1khz.checkIn();
#ifndef WATCHDOG_DISABLED
        monitor1khz.checkForTimeouts();
#endif

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
            const auto res = hw::can::fdcan1.fdcan_transmit(hw::CanMsg{
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

static void FSM_StartAllTasks()
{
    UNUSED(Task1kHz.start());
    UNUSED(Task100Hz.start());
    UNUSED(Task1Hz.start());
    UNUSED(TaskCanTx.start());
    UNUSED(TaskCanRx.start());
}

void tasks_preInit()
{
    hw::bootup::enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

[[noreturn]] void tasks_init()
{
    // __HAL_DBGMCU_FREEZE_IWDG();
    SEGGER_SYSVIEW_Conf();
    hw::can::fdcan1.init();
    adcChipsInit();

    hw::runtimeStat::init(htim7);
    if (const ResetReason reason = hw::resetReason::get(); reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    if (hw::bootup::BootRequest boot_request = hw::bootup::getBootRequest();
        boot_request.context != hw::bootup::BootContext::BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == hw::bootup::BootContext::BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app::can_alerts::infos::StackOverflow_set(true);
        }
        else if (boot_request.context == hw::bootup::BootContext::BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app::can_alerts::infos::WatchdogTimeout_set(true);
        }

        // Clear stack overflow bootup.
        boot_request.context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw::bootup::setBootRequest(boot_request);
    }

    jobs_init();
    osKernelInitialize();
    FSM_StartAllTasks();
    osKernelStart();
    forever {}
}