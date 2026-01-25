#include "jobs.hpp"
#include "app_jsoncan.hpp"
#include "io_canRx.h"
#include "io_canMsgQueues.hpp"
#include "io_time.hpp"

extern "C"
{
    #include "app_canRx.h"
    #include "app_canTx.h"
    #include "io_canTx.h"
    #include "io_canRx.h"
}

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    io::CanMsg msg = app::jsoncan::copyToCanMsg(tx_msg);
    can_tx_queue.pushMsgToQueue(&msg);
}


void jobs_init() {
    app_canTx_init();
    app_canRx_init();

    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode_FDCAN(FDCAN_MODE_DEFAULT, true);

    can_tx_queue.init();
    can_rx_queue.init();
}
void jobs_run1Hz_tick() {
    io_canTx_enqueue1HzMsgs();
}
void jobs_run100Hz_tick() {

    io_canTx_enqueue100HzMsgs();
}
void jobs_run1kHz_tick() {

    const uint32_t task_start_ms = io::time::getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);
}
