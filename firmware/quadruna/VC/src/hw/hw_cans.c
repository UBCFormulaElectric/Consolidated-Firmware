#include "hw_cans.h"
#include "main.h"
#include <assert.h>

const CanHandle can1 = { .hcan = &hfdcan1 };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan);
    return &can1;
}