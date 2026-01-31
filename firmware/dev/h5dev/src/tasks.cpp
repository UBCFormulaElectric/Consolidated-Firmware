#include "tasks.h"

#include <cmsis_os2.h>
#include <io_canRx.h>
#include "main.h"

// io
#include "io_time.hpp"
// hw
#include "hw_hardFaultHandler.hpp"
#include "hw_rtosTaskHandler.hpp"
// old deps
extern "C"
{
#include "hw_bootup.h"
#include "io_canTx.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "hw_resetReason.h"
}

#include "hw_cans.hpp"
#include "io_canMsgQueues.hpp"
#include "app_jsoncan.hpp"
#include "hw_usb.hpp"

[[noreturn]] static void tasks_runCanBroadcast(void *arg)
{
    uint32_t last_1hz = 0, last_100hz = 0;
    forever
    {
        const uint32_t t = io::time::getCurrentMs();
        if (t - last_1hz > 1000)
        {
            io_canTx_enqueue1HzMsgs();
            last_1hz = t;
        }
        else if (t - last_100hz > 10)
        {
            io_canTx_enqueue100HzMsgs();
            last_100hz = t;
        }
        io_canTx_enqueueOtherPeriodicMsgs(t);
    }
}

[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
        io::CanMsg msg = can_tx_queue.popMsgFromQueue();
        LOG_IF_ERR(fdcan1.fdcan_transmit(msg));
    }
}
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        io::CanMsg rx_msg      = can_rx_queue.popMsgFromQueue();
        JsonCanMsg jsoncan_msg = app::jsoncan::copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_msg);
    }
}

// Define the task with StaticTask template class
static hw::rtos::StaticTask<512> TaskCanBroadcast(osPriorityAboveNormal, "Task1Hz", tasks_runCanBroadcast);
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
    fdcan1.init();
    LOG_IF_ERR(hw::usb::init());

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

    app_canTx_init();
    app_canRx_init();

    io_canTx_init(
        [](const JsonCanMsg *tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            can_tx_queue.pushMsgToQueue(&msg);
        });
    io_canTx_enableMode_FDCAN(FDCAN_MODE_DEFAULT, true);

    can_tx_queue.init();
    can_rx_queue.init();
    io_canTx_H5_Bootup_sendAperiodic();

    osKernelInitialize();
    TaskCanBroadcast.start();
    TaskCanRx.start();
    TaskCanTx.start();
    osKernelStart();
    forever {}
}
