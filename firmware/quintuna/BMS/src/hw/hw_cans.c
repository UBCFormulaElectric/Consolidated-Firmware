#include "hw_cans.h"
#include "io_bootHandler.h"
#include "io_canMsg.h"
#include "io_canQueue.h"
#include "main.h"

#include <assert.h>

static void canRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);
    io_canQueue_pushRx(msg);
}

CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 1, .receive_callback = canRxCallback };
CanHandle can2 = { .hcan = &hfdcan2, .bus_num = 2, .receive_callback = canRxCallback };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan || hfdcan == can2.hcan);

    if (hfdcan == can1.hcan)
    {
        return &can1;
    }
    else
    {
        return &can2;
    }
}
