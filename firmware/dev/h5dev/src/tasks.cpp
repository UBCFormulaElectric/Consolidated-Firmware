#include "main.h"
#include "tasks.h"
#include "io_time.hpp"
#include <assert.h>

extern "C"
{
#include "io_canQueue.h"
#include "hw_fdcan.h"
#include "hw_cans.h"
}

static CanTxQueue can_tx_queue;

void tasks_preInit() {}

void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("h5dev reset!");
    hw_can_init(&fdcan);
}

void tasks_run1Hz()
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        io::time::delayUntil(start_time + 1000);
    }
}
void tasks_run100Hz()
{
    forever {}
}
void tasks_run1kHz()
{
    forever {}
}
void tasks_runCanTx()
{
    forever {}
}
void tasks_runCanRx()
{
    forever
    {
        const CanMsg msg = io_canQueue_popRx();
        UNUSED(msg);
    }
}
void tasks_runChimera()
{
    forever
    {
        CanMsg msg = io_canQueue_popTx(&can_tx_queue);
        hw_can_transmit(&fdcan, &msg);
    }
}