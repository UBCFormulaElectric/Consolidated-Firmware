#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"
#include "io_canQueues.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_cans.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        io::time::delayUntil(start_time + 1000);
    }
}
[[noreturn]] static void tasks_run100Hz(void *arg)
{
    forever
    {
        jobs_run100Hz_tick();
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    forever
    {
        jobs_run1kHz_tick();
    }
}
[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
        const auto msg = can_tx_queue.pop();
        if (not msg)
            continue;
        const io::CanMsg &m = msg.value();
        if (m.bus == io::can_tx::BusEnum::Bus_FDCAN)
        {
            const auto res = hw::cans::fdcan1.can_transmit(m);
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
    forever {}
}
// Define the task with StaticTask template class
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityNormal, "TaskCanTx", tasks_runCanTx);

static void CRIT_StartAllTasks()
{
    Task1kHz.start();
    Task1Hz.start();
    Task100Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
}

void tasks_preInit() {}

void tasks_init()
{
    jobs_init();
    osKernelInitialize();
    CRIT_StartAllTasks();
    osKernelStart();
    forever {}
}