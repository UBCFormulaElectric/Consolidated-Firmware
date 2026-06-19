#pragma once
#include "io_queue.hpp"
#include "io_canMsg.hpp"
inline io::queue<io::CanMsg, 256> log_queue("Log Queue");
