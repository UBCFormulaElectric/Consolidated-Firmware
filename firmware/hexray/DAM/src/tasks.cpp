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
#include "hw_bootup.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    constexpr uint32_t period_ms = 1000U;

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
    constexpr uint32_t period_ms = 10U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        jobs_run100Hz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    constexpr uint32_t period_ms = 1U;

    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
#ifndef WATCHDOG_DISABLED
        HAL_IWDG_Refresh(&hiwdg);
#endif
        jobs_run1kHz_tick();
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
    hw::bootup::enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();

    fdcan1.init();

    osKernelInitialize();
    jobs_init();
    DAM_StartAllTasks();
    osKernelStart();
    forever {}
}
