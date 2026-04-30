#include "hw_cans.hpp"
#include "io_canReroutes.hpp"
#include "main.h"
#include <cassert>
#ifndef USE_CHIMERA
#include "io_canQueues.hpp"
#include "app_canUtils.hpp"
#endif

const hw::fdcan fdcan1(hfdcan1, io::canReroute::handleCallback);
const hw::fdcan invcan(hfdcan3, io::canReroute::handleCallback);

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan() || hfdcan == invcan.getHfdcan());
    if (hfdcan == fdcan1.getHfdcan())
    {
        return fdcan1;
    }
    return invcan;
}
