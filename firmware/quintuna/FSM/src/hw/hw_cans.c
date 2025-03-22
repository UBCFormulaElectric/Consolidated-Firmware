#include "hw_cans.h"
#include "main.h"

#include <assert.h>
<<<<<<< HEAD
#include "io_canQueue.h"

CanHandle can = { .hcan = &hcan2, .bus_num = 0, .ready = false, .receive_callback = io_canQueue_pushRx };
=======

CanHandle can = { .hcan = &hcan2, .bus_num = 0, .ready = false };
>>>>>>> 5e8451f7b (Squashed commit of the following:)

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}
