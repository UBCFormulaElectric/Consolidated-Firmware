#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include "shared.pb.h"
#include "jobs.h"

#include <assert.h>

#include "app_canAlerts.h"
#include "app_canDataCapture.h"

#include "io_log.h"
#include "io_canLoggingQueue.h"
#include "io_telemMessage.h"
#include "io_chimera.h"
#include "io_time.h"
#include "io_sbgEllipse.h"
#include "io_fileSystem.h"
#include "io_cans.h"
#include "io_canQueue.h"

#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdogConfig.h"
#include "hw_adcs.h"

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog(void)
{
    if (io_fileSystem_init() == FILE_OK)
        io_canLogging_init();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf(); // aka traceSTART apparently...
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();
    hw_hardFaultHandler_init();
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    hw_adcs_chipsInit();
    // Start interrupt mode for ADC3, since we can't use DMA (see `firmware/quadruna/VC/src/hw/hw_adc.c` for a more
    // in-depth comment).
    HAL_ADC_Start_IT(&hadc3);

    // TODO hw_chimera??
    io_chimera_init(GpioNetName_vc_net_name_tag, AdcNetName_vc_net_name_tag);

    jobs_init();

    // enable these for inverter programming
    // hw_gpio_writePin(&inv_l_program, true);
    // hw_gpio_writePin(&inv_r_program, true);
}

_Noreturn void tasks_run1Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1000U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        jobs_run1Hz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 10;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        jobs_run100Hz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t task_start_ms = io_time_getCurrentMs();

        hw_watchdog_checkForTimeouts();
        jobs_run1kHz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if (io_time_getCurrentMs() - task_start_ms <= period_ms)
            hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanTx(void)
{
    io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        jobs_runCanTx_tick();
    }
}

_Noreturn void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

_Noreturn void tasks_runTelem(void)
{
    for (;;)
    {
        io_telemMessage_broadcastMsgFromQueue();
    }
}

_Noreturn void tasks_runLogging(void)
{
    if (!io_fileSystem_ready())
    {
        // queue shouldn't populate, so this is just an extra precaution
        osThreadSuspend(osThreadGetId());
    }

    static uint32_t write_count         = 0;
    static uint32_t message_batch_count = 0;
    for (;;)
    {
        io_canLogging_recordMsgFromQueue();
        message_batch_count++;
        write_count++;
        if (message_batch_count > 256)
        {
            io_canLogging_sync();
            message_batch_count = 0;
        }
    }
}

/*
 * INTERRUPTS
 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        io_chimera_msgRxCallback();
    }
    else if (huart == &huart2)
    {
        io_sbgEllipse_msgRxCallback();
    }
}
