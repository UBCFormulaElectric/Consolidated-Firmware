#include "hw_cans.h"
#include "io_canQueue.h"
#include "main.h"
#include <assert.h>
#include <stdbool.h>
#include <stm32h733xx.h>

// NOTE: can2 refers to the CAN2_TX/RX on the altium
// I believe CAN2 on the altium refers to can3
CanHandle fd_can  = { .hcan = &hfdcan2, .bus_num = 1, .receive_callback = io_canQueue_pushRx };
CanHandle sx_can  = { .hcan = &hfdcan3, .bus_num = 2, .receive_callback = io_canQueue_pushRx };
CanHandle inv_can = { .hcan = &hfdcan1, .bus_num = 3, .receive_callback = io_canQueue_pushRx };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    if (hfdcan == fd_can.hcan)
    {
        return &fd_can;
    }
    else if (hfdcan == sx_can.hcan)
    {
        return &sx_can;
    }
    else if (hfdcan == inv_can.hcan)
    {
        return &inv_can;
    }
    else
    {
        assert(false);
        return NULL;
    }
}