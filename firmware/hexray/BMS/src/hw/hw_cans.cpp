#include "hw_can.hpp"
#include "jobs.hpp"
#include "io_bootHandler.hpp"
#include "io_canMsg.hpp"
#include "io_canQueue.hpp"
#include <io_canRx.hpp>

#include <cassert>

using namespace hw::cans
{
static void canRxCallback(const CanMsg *msg) {

    io::bootHandler::processBootRequest(msg);

    if (io::canRx::filterMessageId_can1(msg->std_id)) {
        io::canQueue::pushRx(msg);
    }
}

CanHandle can1 = { &hfdcan1, 1, canRxCallback };
CanHandle can2 = { &hfdcan2, 2, canRxCallback };

const CanHandle *hw::can_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == can1.hcan || hfdcan == can2.hcan);

    if (hfdcan == can1.hcan)
    {
        return &can1;
    }
    else
    {
        return &can2;
    }
}
} // namespace hw::cans
