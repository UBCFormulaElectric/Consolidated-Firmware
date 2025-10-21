#include "io/canQueueFake.h"

extern "C"
{
#include "io/canQueue.h"
    void io_canQueue_initRx() {}
    void io_canQueue_initTx(CanTxQueue *queue)
    {
        UNUSED(queue);
    }
    void io_canQueue_pushTx(CanTxQueue *queue, const CanMsg *tx_msg)
    {
        UNUSED(queue);
        UNUSED(tx_msg);
    }
    void io_canQueue_pushRx(const CanMsg *rx_msg)
    {
        UNUSED(rx_msg);
    }
}
