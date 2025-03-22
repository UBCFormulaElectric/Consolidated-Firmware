#include "hw_cans.h"
#include "main.h"

#include <assert.h>
<<<<<<< HEAD
#include "io_canQueue.h"

CanHandle can2 = { .hcan = &hcan2, .bus_num = 0, .receive_callback = io_canQueue_pushRx };
=======

const CanHandle can2 = {
    .hcan    = &hcan2,
    .bus_num = 0,
    .ready   = false,
};
>>>>>>> 5e8451f7b (Squashed commit of the following:)

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can2.hcan);
    return &can2;
}
