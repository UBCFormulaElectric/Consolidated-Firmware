#include "io_canQueues.hpp"

io::queue<io::CanMsg, 128> fdcan_tx_queue("FDCanTXQueue");
io::queue<io::CanMsg, 128> invcan_tx_queue("InvCanTXQueue");
io::queue<io::CanMsg, 128> can_rx_queue("CanRXQueue");
