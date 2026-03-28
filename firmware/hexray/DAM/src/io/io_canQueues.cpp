#include "io_canQueues.hpp"
#include "io_log.hpp"

static void tx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    // LOG_WARN("CAN TX queue overflow");
}
static void rx_overflow_callback(const uint32_t overflow_count)
{
    UNUSED(overflow_count);
    LOG_WARN("CAN RX queue overflow");
}
static void tx_overflow_clear_callback() {}
static void rx_overflow_clear_callback() {}

io::queue<io::CanMsg, 128> can_tx_queue("CanTXQueue", tx_overflow_callback, tx_overflow_clear_callback);
io::queue<io::CanMsg, 128> can_rx_queue("CanRXQueue", rx_overflow_callback, rx_overflow_clear_callback);
