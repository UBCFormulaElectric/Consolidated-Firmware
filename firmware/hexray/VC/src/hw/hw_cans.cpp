#include "hw_cans.hpp"
#include "main.h"
#include <cassert>
#include "io_canQueues.hpp"

const fdcan fdcan1(
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
const fdcan invcan(
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

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan() || hfdcan == invcan.getHfdcan());
    if (hfdcan == fdcan1.getHfdcan())
    {
        return fdcan1;
    }
    return invcan;
}
