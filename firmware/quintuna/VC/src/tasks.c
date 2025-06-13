#include "tasks.h"
#include "hw_bootup.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "io_bootHandler.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_jsoncan.h"

#include "io_log.h"
#include "io_canQueues.h"
#include "io_time.h"
// hw
#include "hw_usb.h"
#include "hw_resetReason.h"
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"

void tasks_preInit(void)
{
    hw_hardFaultHandler_init();
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    ASSERT_EXIT_OK(hw_usb_init());
    hw_can_init(&can1);
    hw_can_init(&can2);
    hw_can_init(&can3);

    // hw_gpio_writePin(&f_pump_en, true);
    // hw_gpio_writePin(&rr_pump_en, true);
    // hw_gpio_writePin(&rl_pump_en, true);

    jobs_init();

    app_canTx_VC_ResetReason_set((CanResetReason)hw_resetReason_get());

    // Check for stack overflow on a previous boot cycle and populate CAN alert.
    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
    {
        // app_canAlerts_VC_Info_StackOverflow_set(true);
        app_canTx_VC_StackOverflowTask_set(boot_request.context_value);

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }
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

_Noreturn void tasks_runCan1Tx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can1_tx_queue);
        hw_fdcan_transmit(&can1, &tx_msg);
    }
}

_Noreturn void tasks_runCan2Tx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can2_tx_queue);
        hw_can_transmit(&can2, &tx_msg);
    }
}

_Noreturn void tasks_runCan3Tx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can3_tx_queue);
        hw_can_transmit(&can3, &tx_msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg rx_msg       = io_canQueue_popRx();
        JsonCanMsg   json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_can_msg);
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
