#include "hw_cans.h"
#include "main.h"

#include <assert.h>

CanHandle can1 = { .hcan = &hcan2, .bus_num = 0, .ready = false };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can1.hcan);
    return &can1;
}