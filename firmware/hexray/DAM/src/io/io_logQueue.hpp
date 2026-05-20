#pragma once
#include "io_queue.hpp"
#include "io_canMsg.hpp"
inline io::queue<io::CanMsg, 52> log_queue("Log Queue");
