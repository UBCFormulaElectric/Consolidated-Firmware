#include "tasks.h"
#include "main.h"
#include "jobs.h"
#include "cmsis_os.h"

#include "hw_adcs.h"
#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_pwmInput.h"
#include "hw_watchdogConfig.h"
#include "hw_watchdog.h"
#include "hw_uarts.h"
#include "hw_crc.h"
#include "hw_cans.h"

#include "io_canTx.h"
#include "io_sd.h"
#include "io_imd.h"
#include "io_tractiveSystem.h"
#include "io_log.h"
#include "io_chimera.h"
#include "io_canQueue.h"

#include "app_canRx.h"
#include "app_accumulator.h"
#include "app_globals.h"
#include "app_stateMachine.h"
#include "utils.h"

#include "shared.pb.h"

static const SdGpio sd_gpio = { .sd_present = {
                                    .port = SD_CD_GPIO_Port,
                                    .pin  = SD_CD_Pin,
                                } };

static const TractiveSystemConfig ts_config = { .ts_vsense_channel_P        = &ts_vsense_p,
                                                .ts_vsense_channel_N        = &ts_vsense_n,
                                                .ts_isense_high_res_channel = &ts_isns_75a,
                                                .ts_isense_low_res_channel  = &ts_isns_400a };

void tasks_preInit(void)
{
    // After booting, re-enable interrupts and ensure the core is using the application's vector table.
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("BMS reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    HAL_TIM_Base_Start(&htim15);

    hw_adcs_chipsInit();

    hw_crc_init(&hcrc);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    hw_can_init(&can1);

    io_imd_init();
    io_tractiveSystem_init(&ts_config);
    io_chimera_init(GpioNetName_bms_net_name_tag, AdcNetName_bms_net_name_tag);
    io_sdGpio_init(&sd_gpio);

    app_globals_init();

    jobs_init();
}

void tasks_deinit(void)
{
    HAL_TIM_Base_Stop_IT(&htim1);
    HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_Stop_IT(&htim15);
    HAL_TIM_Base_DeInit(&htim1);
    HAL_TIM_Base_DeInit(&htim3);
    HAL_TIM_Base_DeInit(&htim15);

    HAL_SD_Abort_IT(&hsd1);
    HAL_SD_DeInit(&hsd1);

    HAL_DMA_Abort_IT(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_adc1);

    HAL_UART_Abort_IT(&huart1);
    HAL_UART_DeInit(&huart1);

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_DeInit(&hadc1);

    HAL_SPI_Abort_IT(&hspi2);
    HAL_SPI_DeInit(&hspi2);

    HAL_CRC_DeInit(&hcrc);
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
        app_stateMachine_tick1Hz();

        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode_Can(CAN_MODE_DEBUG, debug_mode_enabled);
        io_canTx_enqueue1HzMsgs();

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
        app_stateMachine_tick100Hz();
        io_canTx_enqueue100HzMsgs();

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

    static const TickType_t period_ms = 1;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        // Check in for timeouts for all RTOS tasks
        hw_watchdog_checkForTimeouts();
        jobs_run1kHz_tick();

        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
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
        CanMsg tx_msg = io_canQueue_popTx();
        LOG_IF_ERR(hw_can_transmit(&can1, &tx_msg));
    }
}

_Noreturn void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();

    for (;;)
        jobs_runCanRx_tick();
}
