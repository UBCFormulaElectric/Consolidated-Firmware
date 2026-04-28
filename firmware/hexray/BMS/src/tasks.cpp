#include "tasks.h"

#include "app_jsoncan.hpp"

#include "jobs.hpp"
#include "io_time.hpp"
#include <io_canRx.hpp>

#include "hw_adcs.hpp"
#include "hw_watchdog.hpp"
#include "hw_cans.hpp"
#include "hw_error.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "io_canQueues.hpp"
#include <stm32h7xx_hal.h>

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    const uint32_t                 period_ms                = 1000U;
    const uint32_t                 watchdog_grace_period_ms = 50U;
    hw::watchdog::WatchdogInstance watchdog_1hz =
        hw::watchdog::WatchdogInstance(TASK_INDEX_1HZ, period_ms + watchdog_grace_period_ms);
    uint32_t start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_run1Hz_tick();
        watchdog_1hz.checkIn();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_run100Hz(void *arg)
{
    const uint32_t                 period_ms                = 10U;
    const uint32_t                 watchdog_grace_period_ms = 2U;
    hw::watchdog::WatchdogInstance watchdog_100hz =
        hw::watchdog::WatchdogInstance(TASK_INDEX_100HZ, period_ms + watchdog_grace_period_ms);
    uint32_t start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_run100Hz_tick();
        watchdog_100hz.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    const uint32_t                 period_ms                = 1U;
    const uint32_t                 watchdog_grace_period_ms = 1U;
    hw::watchdog::WatchdogInstance watchdog_1khz =
        hw::watchdog::WatchdogInstance(TASK_INDEX_1KHZ, period_ms + watchdog_grace_period_ms);
    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1kHz_tick();
        watchdog_1khz.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
#ifdef CHARGER_CAN
        // Elcon only supports regular CAN but we have some debug messages that are >8 bytes long. Use FDCAN for those
        // (they won't get seen by the charger, but they'll show up on CANoe).
        // TODO: Bit-rate-switching wasn't working for me when the BMS was connected to the charger, so the FD
        // peripheral is configured without BRS. Figure out why it wasn't working?

        const auto msg = can_tx_queue.pop();
        LOG_INFO("message popped");
        if (not msg)
            continue;
        if (const auto &m = msg.value(); m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = hw::can::fdcan2.fdcan_transmit(hw::CanMsg{
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

#else

#endif
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
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityBelowNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityBelowNormal, "TaskCanTx", tasks_runCanTx);

void BMS_StartAllTasks()
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
    __HAL_DBGMCU_FREEZE_IWDG1();
    hw::adc::chipsInit();
    hw::can::fdcan1.init();
    hw::can::fdcan2.init();

    jobs_init();
    osKernelInitialize();
    BMS_StartAllTasks();
    osKernelStart();
    Error_Handler();
}
