#include "hw_cans.h"
#include "main.h"
#include <assert.h>
#include "io_canQueue.h"

CanHandle can1 = { .hcan = &hfdcan2, .bus_num = 0, .receive_callback = io_canQueue_pushRx };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan);
    return &can1;
}
