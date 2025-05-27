#include "tasks.h"

#include "SEGGER_SYSVIEW.h"
#include "cmsis_os.h"
#include "io_log.h"
#include "hw_fdcan.h"
#include "io_canQueue.h"
#include "io_canLogging.h"
#include "hw_hardFaultHandler.h"
#include "io_fileSystem.h"
#include "main.h"
#include "fdcan_spam.h"
#include <assert.h>

static void can_msg_received_callback(const CanMsg *rx_msg)
{
    // TODO: check gpio present
    static uint32_t id = 0;
    // rx_msg->std_id     = id;
    id++;
    io_canQueue_pushRx(rx_msg);
    io_canLogging_loggingQueuePush(rx_msg);
}

CanHandle        can = { .hcan = &hfdcan1, .receive_callback = can_msg_received_callback };
const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can.hcan);
    return &can;
}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h7dev reset!");
    hw_hardFaultHandler_init();
    hw_can_init(&can);
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
        hw_can_transmit(&can, &msg);
    }
}
