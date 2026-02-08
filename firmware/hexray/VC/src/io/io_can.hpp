#pragma once

#include "io_canMsg.hpp"
#include "io_queue.hpp"

namespace io
{
extern queue<io::CanMsg, 128> can1_tx_queue;
extern queue<io::CanMsg, 128> can3_tx_queue;
} // namespace io
