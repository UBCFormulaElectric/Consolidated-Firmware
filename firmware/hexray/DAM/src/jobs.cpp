#include "jobs.hpp"

#include "io_canQueues.hpp"
#include "io_log.hpp"
#include "io_telemMessage.hpp"
#include "io_queue.hpp"
#include "hw_uarts.hpp"

#include <span>

namespace
{
static void telemOverflow(uint32_t)
{
    LOG_WARN("Telem TX queue overflow");
}
static void                                  telemOverflowClear() {}
io::queue<io::telemMessage::TelemCanMsg, 52> telem_tx_queue{ "Telem TX Queue", telemOverflow, telemOverflowClear };
} // namespace

void jobs_init()
{
    can_tx_queue.init();
    can_rx_queue.init();
    telem_tx_queue.init();
}

void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick() {}
void jobs_run1kHz_tick() {}
void jobs_runLogging_tick() {}

void jobs_runTelem_tick()
{
    if (auto result = telem_tx_queue.pop())
    {
        const auto &msg = *result;
        LOG_IF_ERR(_900k_uart.transmitPoll(
            std::span<const uint8_t>{ reinterpret_cast<const uint8_t *>(&msg), msg.wireSize() }));
    }
}

void jobs_runTelemRx()
{
    // TODO: NTP time-sync response — see quintuna DAM io_telemRx.c for protocol
    // Poll _900k_uart.receivePoll(...) for 0xFF 0x01 header, extract IoRtcTime, call io_rtc_setTime()
}

void jobs_runCanTx_tick() {}

void jobs_runCanRx_tick()
{
    if (auto result = can_rx_queue.pop())
    {
        // TODO: io_canRx_updateRxTableWithMessage (once jsoncan is wired up)
        telem_tx_queue.push(io::telemMessage::TelemCanMsg(*result, 0.0f));
    }
}
