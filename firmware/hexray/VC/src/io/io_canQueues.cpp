#include "io_canQueues.hpp"

void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_INFO("CAN TX OVERFLOW");
}
void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_INFO("CAN RX OVERFLOW");
}
void tx_overflow_clear_callback() {}
void rx_overflow_clear_callback() {}

io::queue<io::CanMsg, 128> fdcan_tx_queue("FDCanTXQueue", tx_overflow_callback, tx_overflow_clear_callback);
io::queue<io::CanMsg, 128> invcan_tx_queue("InvCanTXQueue", tx_overflow_callback, tx_overflow_clear_callback);
io::queue<io::CanMsg, 128> can_rx_queue("CanRXQueue", rx_overflow_callback, rx_overflow_clear_callback);
