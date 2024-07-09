#include "hw_cans.h"
#include "io_canMsgQueues.h"
#include "hw_hal.h"

extern CAN_HandleTypeDef hcan1;

namespace hw
{
const can can1{ &hcan1, [](CanMsg *canMsg) { io::can1queue.pushRxMsgToQueue(canMsg); } };
} // namespace hw

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
    hw::can1.receive(CAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
    hw::can1.receive(CAN_RX_FIFO1);
}
