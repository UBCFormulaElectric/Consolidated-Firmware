#include "tasks.h"
#include "hw_pwmOutputs.hpp"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include "io_time.hpp"
#include "io_canRx.hpp"
#include "io_canQueues.hpp"

#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "main.h"
#include "app_canAlerts.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
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

        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run100Hz(void *arg)
{
    constexpr uint32_t             period_ms                = 10U;
    constexpr uint32_t             watchdog_grace_period_ms = 2U;
    hw::watchdog::WatchdogInstance watchdog100hz{ period_ms + watchdog_grace_period_ms };
    hw::watchdog::monitor          monitor100hz{ &watchdog100hz, hiwdg, HAL_IWDG_Refresh };
    monitor100hz.registerWatchdogInstance();

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run100Hz_tick();

        watchdog100hz.checkIn();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
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

        monitor1khz.checkForTimeouts();
        watchdog1khz.checkIn();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_runCanTx(void *arg)
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
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        const auto msg = can_rx_queue.pop();
        if (not msg)
            continue;
        io::can_rx::updateRxTableWithMessage(app::jsoncan::copyFromCanMsg(msg.value()));
    }
}
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
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    // __HAL_DBGMCU_FREEZE_IWDG();
    SEGGER_SYSVIEW_Conf();
    fdcan1.init();
    LOG_IF_ERR(led_dimming.start());
    LOG_IF_ERR(led_dimming.setDutyCycle(95));
    ResetReason reason = hw::resetReason::get();
    if (reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    jobs_init();
    osKernelInitialize();
    CRIT_StartAllTasks();
    osKernelStart();
    forever {}
}