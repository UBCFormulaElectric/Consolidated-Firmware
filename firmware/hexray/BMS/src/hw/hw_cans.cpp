#include "hw_cans.hpp"
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