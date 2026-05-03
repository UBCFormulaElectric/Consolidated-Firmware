#include "hw_cans.hpp"

#include "io_bootHandler.hpp"
#include "bootloader_RSM.hpp"
#include "main.h"

#include <cassert>

constexpr hw::fdcan can1(
    hfdcan1,
    [](const hw::CanMsg &msg)
    {
        io::bootHandler::processBootRequest(msg, board_highbits);
        UNUSED(msg);
    });

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan());
    return can1;
}
