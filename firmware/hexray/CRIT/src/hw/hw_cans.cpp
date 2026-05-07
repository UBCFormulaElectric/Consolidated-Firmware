#include "hw_cans.hpp"
#include "main.h"
#include <cassert>

#ifndef USE_CHIMERA
#include "app_jsoncan.hpp"
#include "io_bootHandler.hpp"
#include "io_canQueues.hpp"
#include "bootloader_CRIT.hpp"
#endif

static void canRxCallback(const hw::CanMsg &msg)
{
#ifndef USE_CHIMERA
    io::bootHandler::processBootRequest(msg, board_highbits);
    LOG_IF_ERR(
        can_rx_queue.push(io::CanMsg{ msg.std_id, msg.dlc, msg.data, true, app::can_utils::BusEnum::Bus_FDCAN }));
#else
    UNUSED(msg);
#endif
}

constexpr hw::fdcan fdcan1{ hfdcan1, canRxCallback };

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan());
    return fdcan1;
}
