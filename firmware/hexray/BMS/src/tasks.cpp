#include "tasks.hpp"

#include "app_jsoncan.hpp"

#include "jobs.hpp"
#include "io_time.hpp"
#include <io_canRx.hpp>

#include "hw_cans.hpp"
#include "hw_error.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "io_canQueues.hpp"

#include "hw_watchdog.hpp"

using namespace hw::watchdog;

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    const uint32_t period_ms = 1000U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run1Hz_tick();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_run100Hz(void *arg)
{
    const uint32_t period_ms = 10U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run100Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    const uint32_t period_ms = 1U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
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

[[noreturn]] static void tasks_runAdbmsVoltages(void *arg) {
    const uint32_t period_ms = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSVOLTAGES, period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsVoltages_tick();

        watchdog.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runAdbmsTemperatures(void *arg) {
    const uint32_t period_ms = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSTEMPERATURES, period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsTemperatures_tick();

        watchdog.checkIn();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runAdbmsDiagnostics(void *arg) {
    const uint32_t period_ms = 500U;
    const uint32_t watchdog_grace_period_ms = 25U;
    WatchdogInstance watchdog = WatchdogInstance(TaskIndex_e::TASK_INDEX_ADBMSDIAGNOSTICS, period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_runAdbmsDiagnostics_tick();

        watchdog.checkIn();
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
static hw::rtos::StaticTask<512> TaskAdbmsTemperatures(osPriorityNormal, "TaskAdbmsTemperatures", tasks_runAdbmsTemperatures);
static hw::rtos::StaticTask<512> TaskAdbmsDiagnostics(osPriorityNormal, "TaskAdbmsDiagnostics", tasks_runAdbmsDiagnostics);


void BMS_StartAllTasks()
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskAdbmsVoltages.start();
    TaskAdbmsTemperatures.start();
    TaskAdbmsDiagnostics.start();
}

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    hw::can::fdcan1.init();
    hw::can::fdcan2.init();

    jobs_init();
    osKernelInitialize();
    BMS_StartAllTasks();
    osKernelStart();
    Error_Handler();
}
