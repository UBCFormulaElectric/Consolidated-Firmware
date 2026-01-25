#include "main.h"
#include <cassert>
#include <io_canQueue.h>
#include "hw_cans.hpp"
#include "io_bootHandler.h"
#include "io_canMsgQueues.hpp"
#include "io_canMsg.hpp"

static void canRxCallback(const io::CanMsg *msg)
{
    can_rx_queue.pushMsgToQueue(msg);
}

hw::fdcan fdcan1(hfdcan1, 0, canRxCallback);

const hw::fdcan &hw::fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan)
{
    assert(hfdcan == fdcan1.getHfdcan());
    return fdcan1;
}