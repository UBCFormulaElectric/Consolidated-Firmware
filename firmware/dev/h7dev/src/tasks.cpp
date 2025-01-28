#include "tasks.h"

extern "C"
{
#include "SEGGER_SYSVIEW.h"
#include "cmsis_os.h"
#include "io_log.h"
#include "io_can.h"
#include "io_canQueue.h"
#include "io_canLoggingQueue.h"
#include "hw_hardFaultHandler.h"
#include "io_fileSystem.h"
#include "hw_sds.h"
#include "main.h"
}

#include "fdcan_spam.h"

CanHandle can{ .hcan = &hfdcan1 };

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h7dev reset!");
    hw_hardFaultHandler_init();
    io_can_init(&can);
    io_canQueue_init();
    if (io_fileSystem_init() == FILE_OK)
        io_canLogging_init();
    // __HAL_DBGMCU_FREEZE_IWDG();
}

void tasks_default()
{
    for (;;)
    {
        fd_can_demo_tick();
    }
}

void tasks_canRx()
{
    for (;;)
    {
        const CanMsg msg = io_canQueue_popRx();
        UNUSED(msg);
    }
}

void tasks_canTx()
{
    for (;;)
    {
        CanMsg msg = io_canQueue_popTx();
        io_can_transmit(&can, &msg);
    }
}

static void can_msg_received_callback(CanMsg *rx_msg)
{
    // TODO: check gpio present
    static uint32_t id = 0;
    rx_msg->std_id     = id;
    id++;
    io_canQueue_pushRx(rx_msg);
    io_canLogging_loggingQueuePush(rx_msg);
}
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo0ITs)
{
    UNUSED(RxFifo0ITs);
    CanMsg rx_msg;
    if (!io_can_receive(&can, FDCAN_RX_FIFO0, &rx_msg))
        // Early return if RX msg is unavailable.
        return;
    can_msg_received_callback(&rx_msg);
}
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, const uint32_t RxFifo1ITs)
{
    UNUSED(RxFifo1ITs);
    CanMsg rx_msg;
    if (!io_can_receive(&can, FDCAN_RX_FIFO1, &rx_msg))
        // Early return if RX msg is unavailable.
        return;
    can_msg_received_callback(&rx_msg);
}
