#include "tasks.h"
#include "hw_watchdog.h"
#include "jobs.h"
#include "cmsis_os.h"
#include "main.h"

#include "app_canTx.h"
#include "app_utils.h"
#include "app_canAlerts.h"
#include "app_jsoncan.h"

// io
#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_bootHandler.h"

#include "app_jsoncan.h"
// chimera
#include "hw_chimera_v2.h"
#include "hw_chimeraConfig_v2.h"

// hw
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_resetReason.h"
#include "hw_usb.h"

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    hw_adcs_chipsInit();
    hw_can_init(&can2);
    ASSERT_EXIT_OK(hw_usb_init());

    const ResetReason reset_reason = hw_resetReason_get();
    app_canTx_RSM_ResetReason_set((CanResetReason)reset_reason);

    // Check for watchdog timeout on a previous boot cycle and populate CAN alert.
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app_canAlerts_RSM_Info_WatchdogTimeout_set(true);
    }

    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context != BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app_canAlerts_RSM_Info_StackOverflow_set(true);
            app_canTx_RSM_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app_canTx_RSM_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    jobs_init();

    io_canTx_RSM_Bootup_sendAperiodic();
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz()
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

_Noreturn void tasks_run100Hz()
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

_Noreturn void tasks_run1kHz()
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

void tasks_runCanTx()
{
    // Setup tasks.
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx(&can_tx_queue);
        LOG_IF_ERR(hw_can_transmit(&can2, &msg));
    }
}

void tasks_runCanRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);

    if (io_canRx_filterMessageId_can2(msg->std_id))
    {
        io_canQueue_pushRx(msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg rx_msg   = io_canQueue_popRx();
        JsonCanMsg   json_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_msg);
    }
}
