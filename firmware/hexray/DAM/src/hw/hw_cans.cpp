#include "hw_cans.hpp"
#include "main.h"
#include <cassert>
namespace hw::cans
{
const fdcan fdcan1(hfdcan1, [](const CanMsg &msg) { UNUSED(msg); });
}
const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::cans::fdcan1.getHfdcan());
    return hw::cans::fdcan1;
}
