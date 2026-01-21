#include "main.h"
#include <cassert>
#include <io_canQueue.h>
#include "hw_cans.hpp"
#include "io_bootHandler.h"
#include "io_canMsgQueues.hpp"
#include "io_canMsg.hpp"

static void canRxCallback(const io::CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);
    io::can_rx_queue.pushTxMsgToQueue(msg);
}

hw::fdcan fdcan = { .hfdcan = &hfdcan1, .bus_num = 0, .receive_callback = canRxCallback };

const hw::fdcan *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan.getHfdcan());
    return &fdcan;
}