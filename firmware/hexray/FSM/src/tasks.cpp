#include "tasks.h"
#include "hw_adcs.hpp"
#include "jobs.hpp"

#include "app_jsoncan.hpp"

#include <io_canRx.hpp>
#include "io_canQueues.hpp"
#include "io_time.hpp"

#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"

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
        float voltage = hw::adcs::susp_fl.getVoltage();
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
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx);

// CFUNC void FSM_StartAllTasks(void)
static void FSM_StartAllTasks()
{
    Task1kHz.start();
    Task100Hz.start();
    Task1Hz.start();
    TaskCanTx.start();
    TaskCanRx.start();
}

void tasks_preInit() {}

[[noreturn]] void tasks_init()
{
    hw::can::fdcan1.init();
    hw::adcs::chipsInit();
    jobs_init();
    osKernelInitialize();
    FSM_StartAllTasks();
    osKernelStart();
    forever {}
}