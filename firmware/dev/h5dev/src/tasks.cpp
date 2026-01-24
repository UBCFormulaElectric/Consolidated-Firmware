#include "tasks.h"

#include <cassert>
#include "main.h"
#include "jobs.hpp"

// io
#include "io_time.hpp"
// hw
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
// old deps
extern "C"
{
#include "hw_bootup.h"
#include "hw_resetReason.h"
#include "io_canQueue.h"
#include "app_jsoncan.h"
}

#include "hw_cans.hpp"
#include "io_canMsgQueues.hpp"

[[noreturn]] static void tasks_run1Hz(void *arg)
{
    uint32_t start_ticks = osKernelGetTickCount();
    forever
    {
        constexpr uint32_t period_ms = 1000U;
        jobs_run1Hz_tick();
        start_ticks += period_ms;
        io::time::delayUntil(start_ticks);
    }
}
[[noreturn]] static void tasks_run100Hz(void *arg)
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
[[noreturn]] static void tasks_run1kHz(void *arg)
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
[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
        io::CanMsg msg = can_tx_queue.popTxMsgFromQueue();
        LOG_IF_ERR(fdcan1.fdcan_transmit(msg));
    }
}
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        // io::CanMsg     rx_msg = can_rx_queue.popRxMsgFromQueue();
    }
}

// Define the task with StaticTask template class
static hw::rtos::StaticTask<512> Task1kHz(osPriorityRealtime, "Task1kHz", tasks_run1kHz);
static hw::rtos::StaticTask<512> Task1Hz(osPriorityAboveNormal, "Task1Hz", tasks_run1Hz);
static hw::rtos::StaticTask<512> Task100Hz(osPriorityHigh, "Task100Hz", tasks_run100Hz);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityBelowNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityBelowNormal, "TaskCanTx", tasks_runCanTx);

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
    can_tx_queue.init();
    can_rx_queue.init();
    fdcan1.init();
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

    osKernelInitialize();
    Task1kHz.start();
    Task100Hz.start();
    Task1Hz.start();
    TaskCanRx.start();
    TaskCanTx.start();
    osKernelStart();
    forever {}
}