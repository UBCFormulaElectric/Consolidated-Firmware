#include "tasks.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_time.h"
// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"
#include "shared.pb.h"

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf(); // aka traceSTART apparently...
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    hw_usb_init();

    jobs_init();
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000U;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run1Hz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz(void)
{
    static const TickType_t period_ms = 10;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run100Hz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    // io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1U;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t task_start_ms = io_time_getCurrentMs();

        // hw_watchdog_checkForTimeouts();
        if (!hw_chimera_v2_enabled)
            jobs_run1kHz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        // if (io_time_getCurrentMs() - task_start_ms <= period_ms)
        //     hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanTx(void)
{
    osDelay(osWaitForever);

    for (;;)
    {
        // CanMsg tx_msg = io_canQueue_popTx();
        // hw_can_transmit(&can1, &tx_msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    // io_chimera_sleepTaskIfEnabled();
    osDelay(osWaitForever);

    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

_Noreturn void tasks_batteryMonitoring(void)
{
    osDelay(osWaitForever);
    for (;;)
    {
        osDelay(1000);
    }
}

_Noreturn void tasks_powerMonitoring(void)
{
    osDelay(osWaitForever);
    for (;;)
    {
        jobs_pollPwrMtr();
    }
}
