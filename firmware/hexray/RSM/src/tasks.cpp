#include "tasks.h"
#include "hw_adcs.hpp"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include "io_time.hpp"
#include "io_canQueues.hpp"
#include <io_canRx.hpp>
#include <io_canTx.hpp>
#include <stm32h5xx_hal.h>

#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"
#include "main.h"
#include <optional>
#include "hw_watchdog.hpp"

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
        io::can_tx::enqueue100HzMsgs();

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

        // monitor1khz.checkForTimeouts();
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
            const auto res = hw::can::can1.fdcan_transmit(hw::CanMsg{
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

// Define the task with StaticTask Template Class
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityLow, "TaskCanRx", tasks_runCanRx);

static void RSM_StartAllTasks()
{
    Task1kHz.start();
    Task100Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
    Task1Hz.start();
}

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    // __HAL_DBGMCU_FREEZE_IWDG();
    hw::adcs::chipsInit();
    hw::can::can1.init();
    jobs_init();
    osKernelInitialize();
    RSM_StartAllTasks();
    osKernelStart();
    int count = 1;
    if (count == 2)
    {
        LOG_WARN("MCU is reset");
    }
    count++;
    
    
    forever {}
}