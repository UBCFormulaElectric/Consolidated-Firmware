#include "hw_cans.h"
#include "main.h"
#include "jobs.h"

#include <assert.h>

CanHandle can1 = { .hcan = &hfdcan1, 0, jobs_runCanRx_callBack };

const CanHandle *hw_can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan);
    return &can1;
}
