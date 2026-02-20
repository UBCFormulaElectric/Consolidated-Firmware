#include "hw_can.hpp"
#include "io_canMsg.hpp"
#include "main.h"

#include <cassert>

namespace hw::can
{
static void canRxCallback(const CanMsg &msg)
{
    UNUSED(msg);
    // io::bootHandler::processBootRequest(msg);

    // if (io::canRx::filterMessageId_can1(msg->std_id))
    // {
    //     io::canQueue::pushRx(msg);
    // }
}

constexpr fdcan can1{ hfdcan1, canRxCallback };
constexpr fdcan can2{ hfdcan2, canRxCallback };

const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.getHfdcan() || hfdcan == can2.getHfdcan());

    if (hfdcan == can1.getHfdcan())
    {
        return can1;
    }
    return can2;
}
} // namespace hw::can
