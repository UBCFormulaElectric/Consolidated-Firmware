#include "hw_cans.hpp"

#include "main.h"
#include "io_canMsgQueues.hpp"
#include "io_canMsg.hpp"
#include "io_bootHandler.hpp"
#include "app_canUtils.hpp"

#include <cassert>

static void canRxCallback(const hw::CanMsg &msg)
{
    io::bootHandler::processBootRequest(msg);
    LOG_IF_ERR(
        can_rx_queue.push(io::CanMsg{ msg.std_id, msg.dlc, msg.data, false, app::can_utils::BusEnum::Bus_FDCAN }));
}

hw::fdcan fdcan1(hfdcan1, canRxCallback);

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan());
    return fdcan1;
}