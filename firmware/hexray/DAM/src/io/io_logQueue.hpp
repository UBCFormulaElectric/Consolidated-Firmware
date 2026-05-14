#pragma once
#include "io_queue.hpp"
#include "io_telemMessage.hpp"
inline io::queue<io::telemMessage::TelemQueueEntry, 52> log_queue("Log Queue");