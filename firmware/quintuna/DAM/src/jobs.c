#include "jobs.h"
#include "io_buzzer.h"
#include "io_log.h"

#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_canMsg.h"

#include "io_telemMessage.h"
#include "io_telemBaseTime.h"

static CanTxQueue can_tx_queue;

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
}

void jobs_init()
{
    io_rtc_init();
    io_canTx_init(jsoncan_transmit_func);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);

    // save start_time to be broadcasted SEE IF THIS SHOULD BE MOVED
    // io_rtc_readTime(&start_time);

    // move into can msg
    io_telemBaseTimeInit();
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
    io_telemBaseTimeSend();
}

void jobs_run100Hz_tick(void)
{
    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg       = io_canQueue_popRx();
    JsonCanMsg   json_can_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&json_can_msg);
}