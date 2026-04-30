#include "hw_cans.hpp"
#include "main.h"
#include <cassert>
#ifndef USE_CHIMERA

#include "app_canUtils.hpp"
#include "app_jsoncan.hpp"
#include "io_canReroute.hpp"
#include "app_canUtils.hpp"
#include "io_canQueues.hpp"

void handleCallback(const hw::CanMsg &hw_rx_msg)
{
    io::CanMsg io_rx_msg{ hw_rx_msg.std_id, hw_rx_msg.dlc, hw_rx_msg.data, true,
                          static_cast<app::can_utils::BusEnum>(0) };

    JsonCanMsg json_can_msg = app::jsoncan::copyFromCanMsg(io_rx_msg);

    io::can_reroute::reroute_InvCAN(json_can_msg);
    io::can_reroute::reroute_FDCAN(json_can_msg);

    can_rx_queue.push(io_rx_msg);
}
#endif

//Chimera cannot interact with app and io layer
const hw::fdcan fdcan1(hfdcan1, 

#ifndef USE_CHIMERA
    handleCallback
#else
[](const hw::CanMsg &msg) { UNUSED(msg); } 
#endif
);

const hw::fdcan invcan(hfdcan3, 
#ifndef USE_CHIMERA
    handleCallback
#else
[](const hw::CanMsg &msg) { UNUSED(msg); } 
#endif
);

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan() || hfdcan == invcan.getHfdcan());
    if (hfdcan == fdcan1.getHfdcan())
    {
        return fdcan1;
    }
    return invcan;
}
