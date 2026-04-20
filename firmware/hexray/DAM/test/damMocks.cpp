#include "io_canQueues.hpp"
#include "io_ntp.hpp"
#include "io_telemRx.hpp"

static void                overflow_callback() {}
static void                overflow_callback(uint32_t) {}
io::queue<io::CanMsg, 128> can_tx_queue{ "", overflow_callback, overflow_callback };
io::queue<io::CanMsg, 128> can_rx_queue{ "", overflow_callback, overflow_callback };

bool io::ntp::wasJustPressed()
{
    return false;
}

void io::telemRx::transmitNTPStartMsg(void) {}