#include "tasks.h"
#include "hw_watchdog.h"
#include "jobs.h"
#include "cmsis_os.h"

// app
#include "app_canTx.h"
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
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_bootup.h"
#include "hw_adcs.h"
#include "hw_resetReason.h"
#include "hw_runTimeStat.h"

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
    LOG_INFO("FSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    ASSERT_EXIT_OK(hw_usb_init());
    hw_adcs_chipsInit();
    hw_can_init(&can);

    hw_runTimeStat_init(&htim7);

    const ResetReason reset_reason = hw_resetReason_get();
    app_canTx_FSM_ResetReason_set((CanResetReason)reset_reason);

    // Check for watchdog timeout on a previous boot cycle and populate CAN alert.
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app_canAlerts_FSM_Info_WatchdogTimeout_set(true);
    }

    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context != BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app_canAlerts_FSM_Info_StackOverflow_set(true);
            app_canTx_FSM_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app_canTx_FSM_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    jobs_init();

    io_canTx_FSM_Bootup_sendAperiodic();
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

void tasks_run1Hz(void)
{
    const uint32_t  period_ms                = 1000U;
    const uint32_t  watchdog_grace_period_ms = 50U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);
    TaskRuntimeStats task_run1hz = { .task_index             = TASK_RUN1HZ,
                                     .cpu_usage_max_setter   = app_canTx_FSM_TaskRun1HzCpuUsageMax_set,
                                     .cpu_usage_setter       = app_canTx_FSM_TaskRun1HzCpuUsage_set,
                                     .stack_usage_max_setter = app_canTx_FSM_TaskRun1HzStackUsage_set };


    hw_runTimeStat_registerTask(&task_run1hz);
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

void tasks_run100Hz(void)
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

void tasks_run1kHz(void)
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
    const uint32_t  period_ms                = 1U;
    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        CanMsg msg = io_canQueue_popTx(&can_tx_queue);
        LOG_IF_ERR(hw_can_transmit(&can, &msg));
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
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
    const uint32_t  period_ms                = 1U;
    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        const CanMsg rx_msg   = io_canQueue_popRx();
        JsonCanMsg   json_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_msg);
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
