#include "hw/cans.h"
#include "io/bootHandler.h"
#include "main.h"

#include <assert.h>
#include <io_canRx.h>
#include "io/canQueue.h"

static void canRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);

    if (io_canRx_filterMessageId_can2(msg->std_id))
    {
        io_canQueue_pushRx(msg);
    }
}

CanHandle can1 = { .hcan = &hcan2, .bus_num = 0, .receive_callback = canRxCallback };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can1.hcan);
    return &can1;
}
