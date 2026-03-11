#include "hw_cans.hpp"
#include "hw_canReroutes.hpp"
#include "main.h"
#include <cassert>

namespace hw::can
{
fdcan fdcan1(hfdcan1, io::canReroute::handleCallback);
fdcan invcan(hfdcan3, io::canReroute::handleCallback);
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
