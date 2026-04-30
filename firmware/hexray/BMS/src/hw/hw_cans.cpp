#include "hw_cans.hpp"
#include "main.h"
#include "io_bootHandler.hpp"
#include "bootloader_BMS.hpp"
#include "io_canQueues.hpp"
#include "app_jsoncan.hpp"

#include <cassert>

static void canRxCallback(const hw::CanMsg &msg)
{
    io::bootHandler::processBootRequest(msg, board_highbits);
    LOG_IF_ERR(
        can_rx_queue.push(io::CanMsg{ msg.std_id, msg.dlc, msg.data, true, app::can_utils::BusEnum::Bus_FDCAN }));
}

constexpr hw::fdcan fdcan1{ hfdcan1, canRxCallback };
constexpr hw::fdcan fdcan2{ hfdcan2, canRxCallback };

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan() || hfdcan == fdcan2.getHfdcan());

    if (hfdcan == fdcan1.getHfdcan())
    {
        return fdcan1;
    }
    return fdcan2;
}