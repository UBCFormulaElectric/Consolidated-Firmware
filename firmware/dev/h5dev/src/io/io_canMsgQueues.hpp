#pragma once
#include "io_canMsgQueue.hpp"
#include "io_canMsg.hpp"

extern io::CanMsgQueue<> can_tx_queue;
extern io::CanMsgQueue<> can_rx_queue;