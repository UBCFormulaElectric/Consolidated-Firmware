#include "tasks.h"
#include "hw_sd.h"
#include "main.h"
#include "cmsis_os.h"
#include "shared.pb.h"
#include "jobs.h"

#include <app_canTx.h>
#include <assert.h>

#include "app_canAlerts.h"
#include "app_canDataCapture.h"
#include "app_utils.h"

#include "io_log.h"
#include "io_canLogging.h"
#include "io_telemMessage.h"
#include "io_chimera.h"
#include "io_time.h"
#include "io_sbgEllipse.h"
#include "io_fileSystem.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"

#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdogConfig.h"
#include "hw_adcs.h"
#include "hw_cans.h"

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog(void)
{
    LOG_INFO("VC reset!");
    io_canLogging_init(NULL);
}

void tasks_deinit(void)
{
    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_Stop_IT(&hadc3);

    HAL_ADC_DeInit(&hadc1);
    HAL_ADC_DeInit(&hadc3);

    HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_DeInit(&htim3);

    HAL_UART_Abort_IT(&huart1);
    HAL_UART_Abort_IT(&huart2);
    HAL_UART_Abort_IT(&huart3);
    HAL_UART_Abort_IT(&huart7);

    HAL_UART_DeInit(&huart1);
    HAL_UART_DeInit(&huart2);
    HAL_UART_DeInit(&huart3);
    HAL_UART_DeInit(&huart7);

    HAL_SD_Abort(&hsd1);
    HAL_SD_DeInit(&hsd1);

    HAL_DMA_Abort(&hdma_adc1);
    HAL_DMA_Abort(&hdma_usart2_rx);

    HAL_DMA_DeInit(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_usart2_rx);
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf(); // aka traceSTART apparently...

    __HAL_DBGMCU_FREEZE_IWDG1();

    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    hw_adcs_chipsInit();
    hw_can_init(&can1); // cast const away in initialization; no mut :(
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
        CanMsg tx_msg = io_canQueue_popTx();
        if (tx_msg.is_fd)
        {
            hw_fdcan_transmit(&can1, &tx_msg);
        }
        else
        {
            LOG_IF_ERR(hw_can_transmit(&can1, &tx_msg));
        }
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
    io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        io_telemMessage_broadcastMsgFromQueue();
    }
}

_Noreturn void tasks_runLogging(void)
{
    io_chimera_sleepTaskIfEnabled();

    static uint32_t write_count         = 0;
    static uint32_t message_batch_count = 0;

    for (;;)
    {
        if (io_canLogging_errorsRemaining() == 0)
        {
            osThreadSuspend(osThreadGetId());
        }

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
