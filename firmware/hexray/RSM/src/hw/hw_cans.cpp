#include "hw_cans.hpp"
#ifndef USE_CHIMERA
#include "io_bootHandler.hpp"
#include "bootloader_RSM.hpp"
#include "io_canQueues.hpp"
#endif
#include "main.h"

#include <cassert>

constexpr hw::fdcan can1(
    hfdcan1,
    [](const hw::CanMsg &msg)
    {
#ifndef USE_CHIMERA
        io::bootHandler::processBootRequest(msg, board_highbits);
        LOG_IF_ERR(can_rx_queue.push({
            msg.std_id,
            msg.dlc,
            msg.data,
            false,
            app::can_utils::BusEnum::Bus_FDCAN,
        }));
#endif
    });

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan());
    return can1;
}
