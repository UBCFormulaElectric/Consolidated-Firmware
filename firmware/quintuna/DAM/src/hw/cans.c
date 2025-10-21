#include "hw/cans.h"
#include "main.h"
#include <assert.h>
#include "io/canQueue.h"
#include "jobs.h"

CanHandle can1 = { .hcan = &hfdcan2, .bus_num = 0, .receive_callback = jobs_runCanRx_callBack };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan);
    return &can1;
}
