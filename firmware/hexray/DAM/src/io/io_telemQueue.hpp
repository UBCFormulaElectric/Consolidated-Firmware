#pragma once

#include "io_queue.hpp"
#include "io_telemMessage.hpp"
#include "io_log.hpp"

inline void telemOverflow(uint32_t)
{
    LOG_WARN("Telem TX queue overflow");
}
inline void                                         telemOverflowClear() {}
inline io::queue<io::telemMessage::TelemCanMsg, 52> telem_tx_queue{ "Telem TX Queue", telemOverflow,
                                                                    telemOverflowClear };