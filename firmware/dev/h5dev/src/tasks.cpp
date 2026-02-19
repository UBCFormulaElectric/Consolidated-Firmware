#include "tasks.h"

#include <cmsis_os2.h>

#include "app_jsoncan.hpp"
#include "io_canRx.hpp"

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
}

#include "hw_cans.hpp"
#include "io_canMsgQueues.hpp"
#include "hw_usb.hpp"

char USBD_PRODUCT_STRING_FS[] = "h5dev";
void hw::usb::receive(const std::span<uint8_t> dest)
{
    LOG_INFO("Recieved a span of length %d", dest.size());
}

[[noreturn]] static void tasks_runCanBroadcast(void *arg)
{
    uint32_t last_1hz = 0, last_100hz = 0;
    osDelay(osWaitForever);
    forever
    {
        const uint32_t t = io::time::getCurrentMs();
        if (t - last_1hz > 1000)
        {
            io::can_tx::enqueue1HzMsgs();
            last_1hz = t;
        }
        else if (t - last_100hz > 10)
        {
            io::can_tx::enqueue100HzMsgs();
            last_100hz = t;
        }
        io::can_tx::enqueueOtherPeriodicMsgs(t);
    }
}

[[noreturn]] static void tasks_runCanTx(void *arg)
{
    forever
    {
        io::CanMsg msg = can_tx_queue.pop().value();
        LOG_IF_ERR(fdcan1.fdcan_transmit({ msg.std_id, msg.dlc, msg.data }));
    }
}
[[noreturn]] static void tasks_runCanRx(void *arg)
{
    forever
    {
        const io::CanMsg rx_msg = can_rx_queue.pop().value();
        io::can_rx::updateRxTableWithMessage(app::jsoncan::copyFromCanMsg(rx_msg));
    }
}

[[noreturn]] static void tasks_loop(void *arg)
{
    forever
    {
        std::array<uint8_t, 8> a{ { 1, 2, 3, 4, 5, 6, 7, 8 } };
        LOG_IF_ERR(hw::usb::transmit(a));
        io::time::delay(1000);
    }
}

// Define the task with StaticTask template class
static hw::rtos::StaticTask<512> TaskCanBroadcast(osPriorityAboveNormal, "Task1Hz", tasks_runCanBroadcast);
static hw::rtos::StaticTask<512> TaskCanRx(osPriorityBelowNormal, "TaskCanRx", tasks_runCanRx);
static hw::rtos::StaticTask<512> TaskCanTx(osPriorityBelowNormal, "TaskCanTx", tasks_runCanTx);
static hw::rtos::StaticTask<512> TaskLoop(osPriorityBelowNormal, "TaskLoop", tasks_loop);

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

    io::can_tx::init(
        [](const JsonCanMsg &tx_msg)
        {
            const io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
            LOG_IF_ERR(can_tx_queue.push(msg));
        });
    io::can_tx::enableMode_FDCAN(app::can_utils::FDCANMode::FDCAN_MODE_DEFAULT, true);

    can_tx_queue.init();
    can_rx_queue.init();
    io::can_tx::H5_Bootup_sendAperiodic();

    osKernelInitialize();
    TaskCanBroadcast.start();
    TaskCanRx.start();
    TaskCanTx.start();
    TaskLoop.start();
    osKernelStart();
    forever {}
}
