#include "hw_cans.h"
#include "main.h"
#include <assert.h>

CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 0 };
CanHandle can2 = { .hcan = &hfdcan2, .bus_num = 0 };
CanHandle can3 = { .hcan = &hfdcan3, .bus_num = 0 };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == can1.hcan) {
        return &can1;
    }
    else if (hfdcan == can2.hcan) {
        return &can2;
    }
    else if (hfdcan == can3.hcan) {
        return &can3;
    }
    else{
        return NULL;
    }
}