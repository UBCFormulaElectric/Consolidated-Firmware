extern "C"
{
#include "io_canTx.h"
    void io_canTx_init(void (*transmit_tx_msg_func)(const JsonCanMsg *))
    {
        (void)transmit_tx_msg_func;
    }
}