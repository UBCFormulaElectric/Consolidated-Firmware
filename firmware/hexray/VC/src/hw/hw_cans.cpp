#include "hw_cans.hpp"
#include "main.h"
#include <cassert>

namespace hw::can
{
fdcan fdcan1(hfdcan1, [](const CanMsg &msg) { UNUSED(msg); });
fdcan invcan(hfdcan3, [](const CanMsg &msg) { UNUSED(msg); });
} // namespace hw::can

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::can::fdcan1.getHfdcan() || hfdcan == hw::can::invcan.getHfdcan());
    if (hfdcan == hw::can::fdcan1.getHfdcan())
    {
        return hw::can::fdcan1;
    }
    return can::invcan;
}
