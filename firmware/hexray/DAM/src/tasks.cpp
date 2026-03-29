#include "tasks.h"
#include "main.h"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include "io_time.hpp"
#include "io_telemMessage.hpp"
#include "io_canQueues.hpp"
#include "io_canQueues.hpp"
#include <io_canRx.hpp>

#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "io_telemQueue.hpp"
#include "hw_watchdog.hpp"
#include "hw_resetReason.hpp"
#include "main.h"
#include "app_canAlerts.hpp"

#include <span>

extern "C"
{
#include "io_rtc.h"
}

// static IoRtcTime boot_time{};
#include "hw_cans.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    const uint32_t                 period_ms                = 1000U;
    const uint32_t                 watchdog_grace_period_ms = 50U;
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
    const uint32_t                 period_ms                = 10U;
    const uint32_t                 watchdog_grace_period_ms = 2U;
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
    const uint32_t                 period_ms                = 1U;
    const uint32_t                 watchdog_grace_period_ms = 1U;
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
            const auto res = hw::can::fdcan1.fdcan_transmit(hw::CanMsg{
                m.std_id,
                m.dlc,
                m.data,
            });
            if (not res)
            {
                LOG_ERROR("fdcan1.fdcan_transmit(...) exited with an error: %d", static_cast<int>(res.error()));
            }
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
static hw::rtos::StaticTask<8096> Task100Hz(osPriorityRealtime, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>  TaskCanTx(osPriorityAboveNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512>  TaskCanRx(osPriorityHigh, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512>  Task1kHz(osPriorityBelowNormal, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512>  Task1Hz(osPriorityBelowNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<1024> TaskLogging(osPriorityHigh, "TaskLogging", tasks_runLogging);
static hw::rtos::StaticTask<512>  TaskTelem(osPriorityHigh, "TaskTelem", tasks_runTelem);
static hw::rtos::StaticTask<512>  TaskTelemRx(osPriorityHigh, "TaskTelemRx", tasks_runTelemRx);

void DAM_StartAllTasks()
{
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1kHz.start();
    Task1Hz.start();
    // TaskLogging.start();
    // TaskTelem.start();
    // TaskTelemRx.start();
}

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    // __HAL_DBGMCU_FREEZE_IWDG();
    hw::can::fdcan1.init();
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
