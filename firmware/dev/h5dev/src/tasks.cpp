#include "main.h"
#include "tasks.h"
#include "io_time.hpp"
#include "jobs.hpp"
#include <cassert>
#include "hw_hardFaultHandler.hpp"

extern "C"
{
#include "hw_bootup.h"
#include "hw_cans.h"
#include "hw_resetReason.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
}

static CanTxQueue can_tx_queue;

void tasks_preInit()
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h5dev reset!");

#ifndef WATCHDOG_DISABLED
    __HAL_DBGMCU_FREEZE_IWDG();
#endif

    hw_hardFaultHandler_init();
    hw_can_init(&fdcan);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);
    // LOG_IF_ERR(hw_usb_init());

    // Check for watchdog timeout on a previous boot cycle.
    if (const ResetReason reset_reason = hw_resetReason_get(); reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
    }

    if (BootRequest boot_request = hw_bootup_getBootRequest(); boot_request.context != BOOT_CONTEXT_NONE)
    {
        if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
        }

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    jobs_init();
    io_canTx_H5_Bootup_sendAperiodic();
}

void tasks_run1Hz()
{
    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        constexpr uint32_t period_ms = 1000U;
        // if (!hw_chimera_v2_enabled)
        // {
        jobs_run1Hz_tick();
        // }
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
    }
}
void tasks_run100Hz()
{
    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        constexpr uint32_t period_ms = 10U;
        jobs_run100Hz_tick();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
    }
}
void tasks_run1kHz()
{
    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        constexpr uint32_t period_ms = 1U;
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
    }
}
void tasks_runCanFDTx()
{
    forever
    {
        CanMsg msg = io_canQueue_popTx(&can_tx_queue);
        LOG_IF_ERR(hw_can_transmit(&fdcan, &msg));
    }
}
void tasks_runCanRx()
{
    forever
    {
        CanMsg     rx_msg         = io_canQueue_popRx();
        JsonCanMsg jsoncan_rx_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}
