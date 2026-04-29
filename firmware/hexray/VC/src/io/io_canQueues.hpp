#pragma once
#include "io_queue.hpp"
#include "io_canMsg.hpp"

extern io::queue<io::CanMsg, 128> fdcan_tx_queue;
extern io::queue<io::CanMsg, 128> invcan_tx_queue;
extern io::queue<io::CanMsg, 128> can_rx_queue;