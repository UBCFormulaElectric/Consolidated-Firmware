#include "jobs.h"

#include "io_canQueue.h"
#include "io_jsoncan.h"

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init()
{
    io_canTx_init(jsoncan_transmit_func);
    io_canQueue_init();
}

void jobs_run1Hz_tick(void) {}

void jobs_run100Hz_tick(void) {}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg       = io_canQueue_popRx();
    JsonCanMsg   json_can_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&json_can_msg);
}