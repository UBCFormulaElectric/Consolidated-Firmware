#include "tasks.h"
#include "hw_hardFaultHandler.h"
#include "io_log.h"
#include "jobs.h"
#include "hw_cans.h"
#include "app_jsoncan.h"
#include "main.h"
#include "io_canQueues.h"
#include <io_canTx.h>
#include <io_canReroute.h>

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}
void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");
    jobs_init();
    __HAL_DBGMCU_FREEZE_IWDG1();
}

_Noreturn void tasks_runCanFDTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&sx_can_tx_queue);
        hw_fdcan_transmit(&fd_can, &tx_msg);
    }
}

_Noreturn void tasks_runCanSxTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&fd_can_tx_queue);
        hw_fdcan_transmit(&sx_can, &tx_msg);
    }
}

_Noreturn void tasks_runCanInvTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&inv_can_tx_queue);
        hw_fdcan_transmit(&inv_can, &tx_msg);
    }
}
_Noreturn void tasks_runcanRx()
{
    for (;;)
    {
        const CanMsg rx_msg       = io_canQueue_popRx();
        JsonCanMsg   json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);

        io_canReroute_can1(&json_can_msg);
        io_canReroute_can2(&json_can_msg);
        io_canReroute_can3(&json_can_msg);
    }
}
