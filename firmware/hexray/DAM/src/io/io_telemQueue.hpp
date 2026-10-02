#pragma once
#include "io_queue.hpp"
#include "io_telemMessage.hpp"
inline io::queue<io::telemMessage::TelemQueueEntry, 512> telem_tx_queue("Telem TX Queue");