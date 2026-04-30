#include "hw_cans.hpp"
#include "main.h"
#include <cassert>

const hw::fdcan fdcan1(hfdcan1, [](const hw::CanMsg &msg) { UNUSED(msg); });
const hw::fdcan invcan(hfdcan3, [](const hw::CanMsg &msg) { UNUSED(msg); });

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan() || hfdcan == invcan.getHfdcan());
    if (hfdcan == fdcan1.getHfdcan())
    {
        return fdcan1;
    }
    return invcan;
}
