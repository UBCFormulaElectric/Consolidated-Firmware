#include "io_canQueueFake.h"

extern "C"
{
#include "io_canQueue.h"
    void io_canQueue_init() {}
    void io_canQueue_pushTx(const CanMsg *tx_msg) {}
    void io_canQueue_pushRx(const CanMsg *rx_msg) {}
}
