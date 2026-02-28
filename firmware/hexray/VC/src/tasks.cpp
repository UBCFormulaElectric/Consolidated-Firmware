#include "tasks.h"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include "io_time.hpp"
#include "io_canQueues.hpp"
#include <io_canRx.hpp>
#include <io_canTx.hpp>

#include "hw_cans.hpp"
#include "hw_rtosTaskHandler.hpp"

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
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

[[noreturn]] static void tasks_runCan1Tx(void *arg)
{
    forever
    {
        const auto msg = fdcan_tx_queue.pop();
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
[[noreturn]] static void tasks_runCan2Tx(void *arg)
{
    forever
    {
        const auto msg = invcan_tx_queue.pop();
        if (not msg)
            continue;
        if (const auto &m = msg.value(); m.bus == app::can_utils::BusEnum::Bus_FDCAN)
        {
            const auto res = hw::can::invcan.can_transmit(hw::CanMsg{
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
static hw::rtos::StaticTask<8096> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512>  Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512>  Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512>  TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512>  TaskCan1Tx(osPriorityNormal, "TaskCanTx", tasks_runCan1Tx);
static hw::rtos::StaticTask<512>  TaskCan2Tx(osPriorityNormal, "TaskCanTx", tasks_runCan2Tx);

static void VC_StartAllTasks()
{
    Task100Hz.start();
    Task1kHz.start();
    Task1Hz.start();
    TaskCanRx.start();
    TaskCan1Tx.start();
    TaskCan2Tx.start();
}

void tasks_preInit() {}

void tasks_init()
{
    jobs_init();
    osKernelInitialize();
    VC_StartAllTasks();
    osKernelStart();
    forever {}
}
