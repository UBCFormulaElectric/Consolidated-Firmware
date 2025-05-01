#include "hw_cans.h"
#include "main.h"

#include <assert.h>
#include "io_canQueue.h"

CanHandle can = { .hcan = &hcan2, .bus_num = 0, .ready = false, .receive_callback = io_canQueue_pushRx };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}
