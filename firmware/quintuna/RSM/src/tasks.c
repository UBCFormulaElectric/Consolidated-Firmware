#include "tasks.h"
#include "jobs.h"
#include "cmsis_os.h"

#include "io_coolants.h"
#include "io_chimera.h"
#include "io_time.h"

#include "hw_pwms.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog() {}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    hw_adcs_chipsInit();

    jobs_init();
}

void tasks_deinit() {}

_Noreturn void tasks_run1Hz()
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1000U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        hw_stackWaterMarkConfig_check();
        jobs_run1Hz_tick();

        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz()
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

        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz()
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

        if (io_time_getCurrentMs() - task_start_ms <= period_ms)
        {
            hw_watchdog_checkIn(watchdog);
        }

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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {}

void HAL_TIM_IC_CaptureCallback()
{
    io_coolant_inputCaptureCallback(&coolant_flow_meter);
}

void canRxQueueOverflowCallBack(uint32_t overflow_count) {}

void canTxQueueOverflowCallBack(uint32_t overflow_count) {}

void canTxQueueOverflowClearCallback(void) {}

void canRxQueueOverflowClearCallback(void) {}

static void jsoncan_transmit() {}