#include "tasks.h"

#include "hw_error.hpp"
#include "hw_hardFaultHandler.hpp"
#include "jobs.hpp"
#include "io_time.hpp"
#include "hw_rtosTaskHandler.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    const uint32_t period_ms                = 1000U;
    const uint32_t watchdog_grace_period_ms = 50U;
    hw::watchdog::WatchdogInstance(TASK_INDEX_1HZ, period_ms + watchdog_grace_period_ms);

    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        hw::watchdog.checkIn();
        io::time::delayUntil(start_time + period_ms);
    }
}

[[noreturn]] static void tasks_run100Hz(void *arg)
{
    const uint32_t period_ms                = 10U;
    const uint32_t watchdog_grace_period_ms = 2U;
    hw::watchdog::WatchdogInstance(TASK_INDEX_100HZ, period_ms + watchdog_grace_period_ms);
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run100Hz_tick();
        hw::watchdog.checkIn();
        io::time::delayUntil(start_time + period_ms);
    }
}
[[noreturn]] static void tasks_run1kHz(void *arg)
{
    const uint32_t period_ms                = 1U;
    const uint32_t watchdog_grace_period_ms = 1U;
    hw::watchdog::WatchdogInstance(TASK_INDEX_1KHZ, period_ms + watchdog_grace_period_ms);
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1kHz_tick();
        hw::watchdog.checkIn();
        io::time::delayUntil(start_time + period_ms);
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

#else

#endif
    }
}
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        jobs_runCanRx_tick();
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
    jobs_init();
    osKernelInitialize();
    BMS_StartAllTasks();
    osKernelStart();
    Error_Handler();
}
