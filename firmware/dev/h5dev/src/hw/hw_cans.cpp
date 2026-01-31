#include "hw_cans.hpp"

#include "main.h"
#include "io_canMsgQueues.hpp"
#include "io_canMsg.hpp"

#include <cassert>

static void canRxCallback(const io::CanMsg &msg)
{
    can_rx_queue.push(msg);
}

hw::fdcan fdcan1(hfdcan1, 0, canRxCallback);

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan());
    return fdcan1;
}