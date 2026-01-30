#include "hw_can.hpp"
#include "jobs.hpp"

extern "C"
{
#include "main.h"
#include <assert.h>
#include "io_canQueue.h"
}

namespace hw
{

fdcan can1(hfdcan2, 0, jobs_runCanRx_callBack);

const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan());
    return can1;
}

} // namespace hw
