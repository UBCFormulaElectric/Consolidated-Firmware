#include "main.h"
#include <assert.h>
#include "hw_cans.h"
#include "io_bootHandler.h"
#include "io_canQueue.h"

static void canRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);
    io_canQueue_pushRx(msg);
}

CanHandle fdcan = { .hcan = &hfdcan1, .bus_num = 0, .receive_callback = canRxCallback };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hcan)
{
    assert(hcan == fdcan.hcan);
    return &fdcan;
}
