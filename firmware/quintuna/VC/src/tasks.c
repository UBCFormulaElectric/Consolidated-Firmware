#include "tasks.h"
#include "hw_bootup.h"
#include "hw_watchdog.h"
#include "hw_gpios.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_jsoncan.h"

#include "io_log.h"
#include "io_time.h"
#include "io_canQueues.h"
// hw
#include "hw_usb.h"
#include "hw_resetReason.h"
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_adcs.h"

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

    hw_adcs_chipsInit();

    const ResetReason reset_reason = hw_resetReason_get();
    app_canTx_VC_ResetReason_set((CanResetReason)reset_reason);

    // Check for watchdog timeout on a previous boot cycle and populate CAN alert.
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app_canAlerts_VC_Info_WatchdogTimeout_set(true);
    }

    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context != BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app_canAlerts_VC_Info_StackOverflow_set(true);
            app_canTx_VC_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app_canTx_VC_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    jobs_init();

    io_canTx_VC_Bootup_sendAperiodic();
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz(void)
{
    const uint32_t  period_ms                = 1000U;
    const uint32_t  watchdog_grace_period_ms = 50U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            jobs_run1Hz_tick();
        }

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz(void)
{
    const uint32_t  period_ms                = 10U;
    const uint32_t  watchdog_grace_period_ms = 2U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            jobs_run100Hz_tick();
        }

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    const uint32_t  period_ms                = 1U;
    const uint32_t  watchdog_grace_period_ms = 1U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        hw_watchdog_checkForTimeouts();

        if (!hw_chimera_v2_enabled)
        {
            jobs_run1kHz_tick();
        }

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

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
    static const TickType_t period_ms = 100U;

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    osDelay(osWaitForever);
    for (;;)
    {
        const uint32_t task_start_ms = io_time_getCurrentMs();

        start_ticks += period_ms;
        osDelay(start_ticks);
    }
}

_Noreturn void tasks_powerMonitoring(void)
{
    static const TickType_t period_ms   = 10;
    static uint32_t         start_ticks = 0;
    start_ticks                         = osKernelGetTickCount();

    for (;;)
    {
        jobs_runPowerMonitoring_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
