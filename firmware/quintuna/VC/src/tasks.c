#include "tasks.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canLoggingQueue.h"
#include "io_time.h"

#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_gpios.h"

#include <io_chimera_v2.h>
#include <shared.pb.h>

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf(); // aka traceSTART apparently...
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    jobs_init();

    // enable these for inverter programming
    // hw_gpio_writePin(&inv_l_program, true);
    // hw_gpio_writePin(&inv_r_program, true);
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
    // io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        // CanMsg tx_msg = io_canQueue_popTx();
        // hw_can_transmit(&can1, &tx_msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    // io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

_Noreturn void tasks_batteryMonitoring(void)
{
    for (;;)
    {
        osDelay(1000);
    }
}