#include "io_canQueues.hpp"

static void overflow_callback() {}
static void overflow_callback(uint32_t) {}

io::queue<io::CanMsg, 128> fdcan_tx_queue{ "", overflow_callback, overflow_callback };
io::queue<io::CanMsg, 128> invcan_tx_queue{ "", overflow_callback, overflow_callback };
io::queue<io::CanMsg, 128> can_rx_queue{ "", overflow_callback, overflow_callback };