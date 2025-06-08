#include "tasks.h"
#include "hw_cans.h"
#include "hw_hardFaultHandler.h"
#include "io_bootloaderReroute.h"
#include "io_canMsg.h"
#include "io_log.h"
#include "jobs.h"
#include "hw_cans.h"
#include "app_jsoncan.h"
#include "main.h"
#include "io_canQueues.h"
#include <io_canTx.h>
#include <io_canReroute.h>
#include <io_canRx.h>

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

bool rx_filter(const CanMsg *msg)
{
    return (io_canRx_filterMessageId_can1(msg->std_id) || io_canRx_filterMessageId_can1(msg->std_id) ||
           io_canRx_filterMessageId_can1(msg->std_id));
}
void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");
    jobs_init();
    hw_can_init(&fd_can);
    hw_can_init(&sx_can);
    hw_can_init(&inv_can);
    __HAL_DBGMCU_FREEZE_IWDG1();
}
_Noreturn void tasks_run10Hz(void)
{
    static const TickType_t period_ms = 100;

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        io_canTx_enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

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
        const CanMsg rx_msg = io_canQueue_popRx();

        io_bootloaderReroute_reRoute(&rx_msg);
        JsonCanMsg json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);

        io_canReroute_can1(&json_can_msg);
        io_canReroute_can2(&json_can_msg);
        io_canReroute_can3(&json_can_msg);
    }
}
