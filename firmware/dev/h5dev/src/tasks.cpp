#include "main.h"
#include "tasks.h"
#include "io_time.hpp"
#include "jobs.hpp"
#include <assert.h>

extern "C"
{
#include "hw_fdcan.h"
#include "hw_cans.h"
#include "hw_bootup.h"
#include "hw_resetReason.h"
// #include "hw_chimera_v2.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
}

static CanTxQueue can_tx_queue;

void tasks_preInit() {}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h5dev reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    hw_can_init(&fdcan);
    // LOG_IF_ERR(hw_usb_init());

    // Check for watchdog timeout on a previous boot cycle.
    const ResetReason reset_reason = hw_resetReason_get();
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
    }

    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context != BOOT_CONTEXT_NONE)
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
    const uint32_t period_ms   = 1000U;
    uint32_t       start_ticks = osKernelGetTickCount();

    forever
    {
        // if (!hw_chimera_v2_enabled)
        // {
        jobs_run1Hz_tick();
        // }

        const uint32_t start_time = io::time::getCurrentMs();
        io::time::delayUntil(start_time + 1000);
    }
}
void tasks_run100Hz()
{
    const uint32_t period_ms   = 10U;
    uint32_t       start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_run100Hz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
void tasks_run1kHz()
{
    const uint32_t period_ms   = 1U;
    uint32_t       start_ticks = osKernelGetTickCount();

    forever
    {
        jobs_run1kHz_tick();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
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
void tasks_runChimera()
{
    forever
    {
        CanMsg msg = io_canQueue_popTx(&can_tx_queue);
        hw_can_transmit(&fdcan, &msg);

        // hw_chimera_v2_task(&chimera_v2_config);
    }
}