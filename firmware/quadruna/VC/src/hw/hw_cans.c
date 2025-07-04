#include "hw_cans.h"
#include <assert.h>
#include "main.h"
#include "jobs.h"

CanHandle can1 = { .hcan = &hfdcan1, .bus_num = 0, .receive_callback = jobs_canRxCallback };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan);
    return &can1;
}
