#include "hw_cans.hpp"
#include "tasks.hpp"
#include <cassert>
extern "C"
{
#include "main.h"
}

namespace hw::cans
{
can fdcan1 = can(hfdcan1, 0, tasks_runCanRxCallback);
} // namespace hw::cans

namespace hw
{
const can &hw_can_getHandle(const FDCAN_HandleTypeDef *hcan)
{
    assert(hcan == hw::cans::fdcan1.getHcan()); // Shared HW CAN Header file should change to use FDCAN???
    return hw::cans::fdcan1;
}
} // namespace hw
