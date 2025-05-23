#include "hw_cans.h"
#include "io_bootHandler.h"
#include "main.h"

#include <assert.h>
#include "io_canQueue.h"

static void canRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);
    io_canQueue_pushRx(msg);
}

CanHandle can2 = { .hcan = &hcan2, .bus_num = 0, .receive_callback = canRxCallback };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can2.hcan);
    return &can2;
}
