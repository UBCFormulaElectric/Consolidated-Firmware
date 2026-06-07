#include "io_canQueues.hpp"
io::queue<io::CanMsg, 250> can_tx_queue("CanTXQueue");
io::queue<io::CanMsg, 128> can_rx_queue("CanRXQueue");
