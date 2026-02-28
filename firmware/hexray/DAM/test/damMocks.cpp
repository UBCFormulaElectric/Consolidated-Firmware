#include "io_canQueues.hpp"

static void                penis() {}
static void                penis(uint32_t) {}
io::queue<io::CanMsg, 128> can_tx_queue{ "", penis, penis };
io::queue<io::CanMsg, 128> can_rx_queue{ "", penis, penis };