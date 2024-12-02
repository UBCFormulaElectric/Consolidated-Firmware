#include "tasks.h"

extern "C"
{
#include "SEGGER_SYSVIEW.h"
#include "io_log.h"
#include "cmsis_os.h"
#include "io_can.h"
}

#include "fdcan_spam.h"

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h7dev reset!");
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
        CanMsg msg;
        io_can_popRxMsgFromQueue(&msg);
    }
}

void tasks_canTx()
{
    for (;;)
    {
        CanMsg msg;
        io_can_popTxMsgFromQueue(&msg);
        hw_can_transmit(&msg);
    }
}
