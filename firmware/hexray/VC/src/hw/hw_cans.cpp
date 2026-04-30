#include "hw_cans.hpp"
#include "main.h"
#include <cassert>
#ifndef USE_CHIMERA
#include "io_canQueues.hpp"
#include "app_canUtils.hpp"
#endif

const hw::fdcan fdcan1(
    hfdcan1,
    [](const hw::CanMsg &msg)
    {
#ifndef USE_CHIMERA
        LOG_IF_ERR(can_rx_queue.push({
            msg.std_id,
            msg.dlc,
            msg.data,
            true,
            app::can_utils::BusEnum::Bus_FDCAN,
        }));
#else
        UNUSED(msg);
#endif
    });
const hw::fdcan invcan(
    hfdcan3,
    [](const hw::CanMsg &msg)
    {
#ifndef USE_CHIMERA
        LOG_IF_ERR(can_rx_queue.push({
            msg.std_id,
            msg.dlc,
            msg.data,
            false,
            app::can_utils::BusEnum::Bus_FDCAN,
        }));
#else
        UNUSED(msg);
#endif
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
