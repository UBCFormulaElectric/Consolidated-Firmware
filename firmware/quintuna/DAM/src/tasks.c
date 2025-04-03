#include "tasks.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"

#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_gpios.h"

#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"
#include <shared.pb.h>

void tasks_preInit(void)
{
    // After booting, re-enable interrupts and ensure the core is using the application's vector table.
    // hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog(void)
{
    // if (io_fileSystem_init() == FILE_OK)
    //     io_canLogging_init();
}

void tasks_init(void)
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("DAM reset!");

    hw_hardFaultHandler_init();
    hw_can_init(&can1);
    hw_usb_init();
    // hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    hw_gpio_writePin(&tsim_red_en_pin, true);
    hw_gpio_writePin(&ntsim_green_en_pin, false);
}

_Noreturn void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000U;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
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
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static const TickType_t period_ms   = 10;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        hw_chimera_v2_mainOrContinue(&chimera_v2_config);
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
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static const TickType_t period_ms   = 1U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        // const uint32_t task_start_ms = io_time_getCurrentMs();

        // hw_watchdog_checkForTimeouts();
        jobs_run1kHz_tick();

        // // Watchdog check-in must be the last function called before putting the
        // // task to sleep. Prevent check in if the elapsed period is greater or
        // // equal to the period ms
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
        CanMsg tx_msg = io_canQueue_popTx();
        hw_can_transmit(&can1, &tx_msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    osDelay(osWaitForever);
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

_Noreturn void tasks_runTelem(void)
{
    osDelay(osWaitForever);
    for (;;)
    {
        // io_telemMessage_broadcastMsgFromQueue();
    }
}

_Noreturn void tasks_runLogging(void)
{
    osDelay(osWaitForever);
    // if (!io_fileSystem_ready())
    // {
    //     // queue shouldn't populate, so this is just an extra precaution
    //     osThreadSuspend(osThreadGetId());
    // }

    // static uint32_t write_count         = 0;
    // static uint32_t message_batch_count = 0;
    for (;;)
    {
        // io_canLogging_recordMsgFromQueue();
        // message_batch_count++;
        // write_count++;
        // if (message_batch_count > 256)
        // {
        //     io_canLogging_sync();
        //     message_batch_count = 0;
        // }
    }
}
