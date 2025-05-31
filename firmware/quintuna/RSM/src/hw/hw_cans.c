#include "hw_cans.h"

#include "main.h"
#include "tasks.h"
#include <assert.h>

CanHandle can2 = { .hcan = &hcan2, .bus_num = 0, .receive_callback = tasks_runCanRxCallback };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can2.hcan);
    return &can2;
}
