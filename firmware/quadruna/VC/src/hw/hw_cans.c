#include "hw_cans.h"
#include "main.h"
#include <assert.h>

CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 0 };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan);
    return &can1;
}