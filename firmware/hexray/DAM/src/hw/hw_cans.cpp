#include "hw_cans.hpp"
#include "main.h"
#include <cassert>

#ifndef USE_CHIMERA
#include "io_bootHandler.hpp"
#include "bootloader_DAM.hpp"
#endif

constexpr hw::fdcan fdcan1(
    hfdcan1,
    [](const hw::CanMsg &msg)
    {
#ifndef USE_CHIMERA
        io::bootHandler::processBootRequest(msg, board_highbits);
#endif
    });

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan());
    return fdcan1;
}
