#include "tasks.h"
#include "jobs.hpp"
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
#include <stm32h7xx_hal.h>
#include "main.h"
#include "hw_resetReason.hpp"
#include "app_canAlerts.hpp"
#include "hw_gpios.hpp"

#include "hw_watchdog.hpp"

using namespace hw::watchdog;

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    constexpr uint32_t             period_ms                = 1000U;
    constexpr uint32_t             watchdog_grace_period_ms = 50U;
    hw::watchdog::WatchdogInstance watchdog1hz{ period_ms + watchdog_grace_period_ms };
    hw::watchdog::monitor          monitor1hz{ &watchdog1hz, hiwdg1, HAL_IWDG_Refresh };
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
    hw::watchdog::monitor          monitor100hz{ &watchdog100hz, hiwdg1, HAL_IWDG_Refresh };
    monitor100hz.registerWatchdogInstance();
    uint32_t start_ticks = osKernelGetTickCount();

    forever
    {
        // const uint32_t start_time = io::time::getCurrentMs();
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
    hw::watchdog::monitor          monitor1khz{ &watchdog1khz, hiwdg1, HAL_IWDG_Refresh };
    monitor1khz.registerWatchdogInstance();

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
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
            std::expected<void, ErrorCode> res;
            if (can_msg.dlc > 8)
                res = fdcan1.fdcan_transmit(can_msg);
            else
                res = fdcan1.can_transmit(can_msg);
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

[[noreturn]] static void tasks_runAdbmsVoltages(void *arg)
{
    const uint32_t period_ms                = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    // WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSVOLTAGES, period_ms +
    // watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsVoltages_tick();

        // watchdog.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runAdbmsFilteredVoltages(void *arg)
{
    const uint32_t period_ms                = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    // WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSVOLTAGES, period_ms +
    // watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsFilteredVoltages_tick();

        // watchdog.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runAdbmsTemperatures(void *arg)
{
    const uint32_t period_ms                = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    // WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSTEMPERATURES, period_ms +
    // watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsTemperatures_tick();

        // watchdog.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runAdbmsDiagnostics(void *arg)
{
    const uint32_t period_ms                = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    // WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSDIAGNOSTICS, period_ms +
    // watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        // const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsDiagnostics_tick();

        // watchdog.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

// Define the task with StaticTask template class
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityBelowNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityBelowNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512> TaskAdbmsVoltages(osPriorityNormal, "TaskAdbmsVoltages", tasks_runAdbmsVoltages);
static hw::rtos::StaticTask<512>
    TaskAdbmsFilteredVoltages(osPriorityNormal, "TaskAdbmsFilteredVoltages", tasks_runAdbmsFilteredVoltages);
static hw::rtos::StaticTask<512>
    TaskAdbmsTemperatures(osPriorityNormal, "TaskAdbmsTemperatures", tasks_runAdbmsTemperatures);
static hw::rtos::StaticTask<512>
    TaskAdbmsDiagnostics(osPriorityNormal, "TaskAdbmsDiagnostics", tasks_runAdbmsDiagnostics);

void BMS_StartAllTasks()
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskAdbmsVoltages.start();
    TaskAdbmsFilteredVoltages.start();
    TaskAdbmsTemperatures.start();
    TaskAdbmsDiagnostics.start();
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
    // __HAL_DBGMCU_FREEZE_IWDG1();
    // hw::adc::chipsInit();
    SEGGER_SYSVIEW_Conf();

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

    hw::bootup::BootRequest boot_request = hw::bootup::getBootRequest();
    if (boot_request.context != hw::bootup::BootContext::BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == hw::bootup::BootContext::BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app::can_alerts::infos::StackOverflow_set(true);
            app::can_tx::BMS_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == hw::bootup::BootContext::BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app::can_alerts::infos::WatchdogTimeout_set(true);
            app::can_tx::BMS_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = hw::bootup::BootContext::BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw::bootup::setBootRequest(boot_request);
    }

    jobs_init();
    jobs_adbms_init();
    osKernelInitialize();
    BMS_StartAllTasks();
    osKernelStart();
    Error_Handler();
}
