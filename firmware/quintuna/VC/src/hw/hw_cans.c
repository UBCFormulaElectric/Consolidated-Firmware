#include "hw_cans.h"
#include "main.h"
#include <assert.h>

// NOTE: can2 refers to the CAN2_TX/RX on the altium
// I believe CAN2 on the altium refers to can3
CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 1 };
CanHandle can2 = { .hcan = &hfdcan3, .bus_num = 2 };
CanHandle can3 = { .hcan = &hfdcan2, .bus_num = 3 };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == can1.hcan)
    {
        return &can1;
    }
    else if (hfdcan == can2.hcan)
    {
        return &can2;
    }
    else if (hfdcan == can3.hcan)
    {
        return &can3;
    }
    else
    {
        assert(false);
        return NULL;
    }
}