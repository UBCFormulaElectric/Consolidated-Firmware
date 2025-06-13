#include "hw_cans.h"
#include "io_bootHandler.h"
#include "io_canMsg.h"
#include "io_canQueue.h"
#include "main.h"

#include <assert.h>
#include <io_canRx.h>

static void can1RxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);

    if (io_canRx_filterMessageId_can1(msg->std_id))
    {
        io_canQueue_pushRx(msg);
    }
}

static void canChargerRxCallback(const CanMsg *msg)
{
    io_bootHandler_processBootRequest(msg);

    if (io_canRx_filterMessageId_charger(msg->std_id))
    {
        io_canQueue_pushRx(msg);
    }
}

CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 1, .receive_callback = can1RxCallback };
CanHandle can2 = { .hcan = &hfdcan2, .bus_num = 2, .receive_callback = canChargerRxCallback };

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
