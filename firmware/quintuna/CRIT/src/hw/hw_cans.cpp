#include "main.h"
#include "hw_cans.h"
#include "io_canMsgQueues.h"

namespace hw::can
{
const CanBus can1{ &hcan1, [](const CanMsg *canMsg)
                   {
                       if (!io_canRx_filterMessageId(canMsg->std_id))
                           return;
                       io::can1queue.pushRxMsgToQueue(canMsg);
                   } };
} // namespace hw::can

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
    hw::can::can1.receive(CAN_RX_FIFO0);
}

extern "C" void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UNUSED(hcan);
    hw::can::can1.receive(CAN_RX_FIFO1);
}
