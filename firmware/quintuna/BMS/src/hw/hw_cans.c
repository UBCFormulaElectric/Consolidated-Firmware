#include "hw_cans.h"
#include "main.h"
#include "io_canQueue.h"

#include <assert.h>

CanHandle can1 = { .hcan = &hfdcan1, .receive_callback = io_canQueue_pushRx };
CanHandle can2 = { .hcan = &hfdcan2, .receive_callback = io_canQueue_pushRx };

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
