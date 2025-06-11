#include "io_canQueueFake.h"

extern "C"
{
#include "io_canQueue.h"
    void io_canQueue_initRx() {}
    void io_canQueue_initTx(CanTxQueue *queue) {}
    void io_canQueue_pushTx(CanTxQueue *queue, const CanMsg *tx_msg) {}
    void io_canQueue_pushRx(const CanMsg *rx_msg) {}
}
