#include "hw/cans.h"
#include "io/bootHandler.h"
#include "io/canQueue.h"
#include "main.h"
#include <assert.h>
#include <io_canRx.h>

static void canRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);

    if (io_canRx_filterMessageId_can1(msg->std_id) || io_canRx_filterMessageId_can2(msg->std_id) ||
        io_canRx_filterMessageId_can3(msg->std_id))
    {
        io_canQueue_pushRx(msg);
    }
}

// NOTE: can2 refers to the CAN2_TX/RX on the altium
// I believe CAN2 on the altium refers to can3
CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 1, .receive_callback = canRxCallback };
CanHandle can2 = { .hcan = &hfdcan3, .bus_num = 2, .receive_callback = canRxCallback };
CanHandle can3 = { .hcan = &hfdcan2, .bus_num = 3, .receive_callback = canRxCallback };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == can1.hcan)
    {
        return &can1;
    }
    else if (hfdcan == can2.hcan)
    {
        return &can2;
    }
    else if (hfdcan == can3.hcan)
    {
        return &can3;
    }
    else
    {
        assert(false);
        return NULL;
    }
}
