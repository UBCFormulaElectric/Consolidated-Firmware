#include "hw_cans.h"
#include "main.h"

#include <assert.h>
<<<<<<< HEAD
#include "io_canQueue.h"

CanHandle can1 = { .hcan = &hcan2, .bus_num = 0, .receive_callback = io_canQueue_pushRx };
=======

CanHandle can1 = { .hcan = &hcan2, .bus_num = 0, .ready = false };
>>>>>>> 5e8451f7b (Squashed commit of the following:)

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can1.hcan);
    return &can1;
<<<<<<< HEAD
}
=======
}
>>>>>>> 5e8451f7b (Squashed commit of the following:)
