#include "main.h"
#include "hw_cans.h"
#include "io_canMsgQueues.h"

namespace hw::can
{
const CanBus can2{ &hcan2, [](const CanMsg *canMsg) { io::can2queue.pushRxMsgToQueue(canMsg); } };
} // namespace hw::can

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
    hw::can::can2.receive(CAN_RX_FIFO0);
}

extern "C" void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
    hw::can::can2.receive(CAN_RX_FIFO1);
}
