#include "tasks.h"
#include "app/utils.h"
#include "hw/cans.h"
#include "hw/hardFaultHandler.h"
#include "hw/resetReason.h"
#include "hw/utils.h"
#include "hw/watchdog.h"
#include "io/bootloaderReroute.h"
#include "io/canMsg.h"
#include "io/log.h"
#include "io/time.h"
#include "jobs.h"
#include "app/jsoncan.h"
#include "main.h"
#include "io/canQueues.h"
#include <app_canTx.h>
#include <cmsis_os2.h>
#include <io_canReroute.h>
#include <io_canRx.h>
#include <io_canTx.h>
#include <app_canAlerts.h>
#include <stdint.h>

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_INFO("VCR tx overflow %d", overflow_count);
}

void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VCR reset!");
    hw_can_init(&fd_can);
    hw_can_init(&sx_can);
    hw_can_init(&inv_can);
    __HAL_DBGMCU_FREEZE_IWDG1();

    const ResetReason reset_reason = hw_resetReason_get();
    app_canTx_VCR_ResetReason_set((CanResetReason)reset_reason);

    // Check for watchdog timeout on a previous boot cycle and populate CAN alert.
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app_canAlerts_VCR_Info_WatchdogTimeout_set(true);
    }

    app_canTx_VCR_ResetReason_set((CanResetReason)hw_resetReason_get());

    jobs_init();

    io_canTx_VCR_Bootup_sendAperiodic();
}

_Noreturn void tasks_run1Hz(void)
{
    const uint32_t  period_ms                = 1000U;
    const uint32_t  watchdog_grace_period_ms = 50U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();

    for (;;)
    {
        // TODO: 100Hz frequency enqueueing.
        io_canTx_enqueue1HzMsgs();

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

        io_canTx_enqueueOtherPeriodicMsgs(start_ticks);

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanFDTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&fd_can_tx_queue);
        hw_fdcan_transmit(&fd_can, &tx_msg);
    }
}

_Noreturn void tasks_runCanSxTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&sx_can_tx_queue);
        hw_can_transmit(&sx_can, &tx_msg);
    }
}

_Noreturn void tasks_runCanInvTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&inv_can_tx_queue);
        hw_can_transmit(&inv_can, &tx_msg);
    }
}
_Noreturn void tasks_runcanRx(void)
{
    for (;;)
    {
        // Doesnt' receive anything via JSONCAN
        osDelay(osWaitForever);
    }
}
