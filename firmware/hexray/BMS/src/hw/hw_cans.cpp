#include "hw_cans.hpp"
#include "main.h"
#include "io_bootHandler.hpp"
#include "bootloader_BMS.hpp"
#include "io_canQueues.hpp"
#include "app_jsonCan.hpp"

#include <cassert>

namespace hw::can
{
static void canRxCallback(const CanMsg &msg)
{
    io::bootHandler::processBootRequest(msg, board_highbits);
    LOG_IF_ERR(can_rx_queue.push(io::CanMsg{ msg.std_id, msg.dlc, msg.data, true, app::can_utils::BusEnum::Bus_FDCAN }));
}

constexpr fdcan fdcan1{ hfdcan1, canRxCallback };
constexpr fdcan fdcan2{ hfdcan2, canRxCallback };
} // namespace hw::can

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == hw::can::fdcan1.getHfdcan() || hfdcan == hw::can::fdcan2.getHfdcan());

    if (hfdcan == hw::can::fdcan1.getHfdcan())
    {
        return hw::can::fdcan1;
    }
    return hw::can::fdcan2;
}