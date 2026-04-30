#include "hw_cans.hpp"
#include "main.h"
#include <cassert>
#include "io_canQueues.hpp"

namespace hw::can
{
fdcan fdcan1(
    hfdcan1,
    [](const CanMsg &msg)
    {
        LOG_IF_ERR(can_rx_queue.push({
            msg.std_id,
            msg.dlc,
            msg.data,
            true,
            app::can_utils::BusEnum::Bus_FDCAN,
        }));
    });
fdcan invcan(
    hfdcan3,
    [](const CanMsg &msg)
    {
        LOG_IF_ERR(can_rx_queue.push({
            msg.std_id,
            msg.dlc,
            msg.data,
            false,
            app::can_utils::BusEnum::Bus_FDCAN,
        }));
    });
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
