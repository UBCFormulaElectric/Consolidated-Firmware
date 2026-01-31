#include "io_canMsgQueues.hpp"
#include <string>

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
void tx_overflow_clear_callback(void) {}
void rx_overflow_clear_callback(void) {}

io::CanMsgQueue<> can_tx_queue("CanTXQueue", tx_overflow_callback, tx_overflow_clear_callback);
io::CanMsgQueue<> can_rx_queue("CanRXQueue", rx_overflow_callback, rx_overflow_clear_callback);