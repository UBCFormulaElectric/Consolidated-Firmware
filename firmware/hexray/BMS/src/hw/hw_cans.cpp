#include "hw_can.hpp"
#include "io_canMsg.hpp"
#include "main.h"

#include <cassert>

namespace hw::can
{
static void canRxCallback(const io::CanMsg &msg)
{
    UNUSED(msg);
    // io::bootHandler::processBootRequest(msg);

    // if (io::canRx::filterMessageId_can1(msg->std_id))
    // {
    //     io::canQueue::pushRx(msg);
    // }
}

const fdcan can1{ hfdcan1, 1, canRxCallback };
const fdcan can2{ hfdcan2, 2, canRxCallback };

const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan() || hfdcan == can2.getHfdcan());

    if (hfdcan == can1.getHfdcan())
    {
        return can1;
    }
    else
    {
        return can2;
    }
}
} // namespace hw::can
