#include "hw/cans.h"

#include "main.h"
#include "tasks.h"
#include <assert.h>

CanHandle can = { .hcan = &hcan2, .bus_num = 0, .receive_callback = tasks_runCanRxCallback };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}
