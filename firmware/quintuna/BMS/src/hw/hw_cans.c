#include "hw_cans.h"
#include "main.h"

#include <assert.h>

CanHandle can1 = { .hcan = &hfdcan1 };
CanHandle can2 = { .hcan = &hfdcan2 };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan || hfdcan == can2.hcan);

    if (hfdcan == can1.hcan) {
        return &can1;
    }
    else {
        return &can2;
    }
}
