#include "io_canMsgQueues.hpp"

io::CanMsgQueue can_tx_queue(
    "CanTXQueue",
    nullptr,
    nullptr,
    nullptr,
    nullptr);


io::CanMsgQueue can_rx_queue(
    "CanRXQueue",
    nullptr,
    nullptr,
    nullptr,
    nullptr);