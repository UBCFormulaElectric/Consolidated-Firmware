#include "hw_cans.hpp"
#ifndef USE_CHIMERA
#include "io_bootHandler.hpp"
#include "bootloader_RSM.hpp"
#endif
#include "main.h"

#include <cassert>

constexpr hw::fdcan can1(
    hfdcan1,
    [](const hw::CanMsg &msg)
    {
#ifndef USE_CHIMERA
        io::bootHandler::processBootRequest(msg, board_highbits);
#endif
    });

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan());
    return can1;
}
