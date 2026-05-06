#include "tasks.h"
#include "main.h"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include "io_time.hpp"
#include "io_telemMessage.hpp"
#include "io_canQueues.hpp"
#include "io_canRx.hpp"
#include "io_telemQueue.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "app_canAlerts.hpp"
#include "hw_bootup.hpp"

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

        watchdog1khz.checkIn();
#ifndef WATCHDOG_DISABLED
        monitor1khz.checkForTimeouts();
#endif

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runLogging(void *arg)
{
    osDelayUntil(osWaitForever);
    forever
    {
        jobs_runLogging_tick();
    }
}
[[noreturn]] static void tasks_runTelem(void *arg)
{
    // const io::telemMessage::BaseTimeRegMsg base_time_msg(boot_time);
    // LOG_IF_ERR(_900k_uart.transmitPoll(
    //     std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&base_time_msg), sizeof(base_time_msg) }));

    forever
    {
        jobs_runTelem_tick();
    }
}
[[noreturn]] static void tasks_runTelemRx(void *arg)
{
    osDelayUntil(osWaitForever);
    forever
    {
        jobs_runTelemRx();
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

// Define the task with StaticTask Class
static hw::rtos::StaticTask::StaticTaskStack<8096> Task100HzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCanTxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskCanRxStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  Task1kHzStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  Task1HzStack;
static hw::rtos::StaticTask::StaticTaskStack<1024> TaskLoggingStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskTelemStack;
static hw::rtos::StaticTask::StaticTaskStack<512>  TaskTelemRxStack;

static hw::rtos::StaticTask Task100Hz(osPriorityRealtime, "Task100Hz", tasks_run100Hz, Task100HzStack);
static hw::rtos::StaticTask TaskCanTx(osPriorityAboveNormal, "TaskCanTx", tasks_runCanTx, TaskCanTxStack);
static hw::rtos::StaticTask TaskCanRx(osPriorityHigh, "TaskCanRx", tasks_runCanRx, TaskCanRxStack);
static hw::rtos::StaticTask Task1kHz(osPriorityBelowNormal, "Task1kHz", tasks_run1kHz, Task1kHzStack);
static hw::rtos::StaticTask Task1Hz(osPriorityBelowNormal, "Task1Hz", tasks_run1Hz, Task1HzStack);
static hw::rtos::StaticTask TaskLogging(osPriorityHigh, "TaskLogging", tasks_runLogging, TaskLoggingStack);
static hw::rtos::StaticTask TaskTelem(osPriorityHigh, "TaskTelem", tasks_runTelem, TaskTelemStack);
static hw::rtos::StaticTask TaskTelemRx(osPriorityHigh, "TaskTelemRx", tasks_runTelemRx, TaskTelemRxStack);

static void DAM_StartAllTasks()
{
    UNUSED(Task100Hz.start());
    UNUSED(TaskCanTx.start());
    UNUSED(TaskCanRx.start());
    UNUSED(Task1kHz.start());
    UNUSED(Task1Hz.start());
    // TaskLogging.start();
    // TaskTelem.start();
    // TaskTelemRx.start();
}

void tasks_preInit()
{
    hw::bootup::enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    // __HAL_DBGMCU_FREEZE_IWDG();
    SEGGER_SYSVIEW_Conf();
    fdcan1.init();
    ResetReason reason = hw::resetReason::get();
    if (reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app::can_alerts::infos::WatchdogTimeout_set(true);
    }

    osKernelInitialize();
    jobs_init();
    DAM_StartAllTasks();
    osKernelStart();
    forever {}
}
