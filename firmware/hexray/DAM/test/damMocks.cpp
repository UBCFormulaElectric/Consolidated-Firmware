#include "io_canQueues.hpp"
#include "io_ntpButton.hpp"
#include "io_telemRx.hpp"
#include <expected>
#include <util_errorCodes.hpp>

io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };
bool                       io::ntpButton::wasJustPressed()
{
    return false;
}

std::expected<io::rtc::Time, ErrorCode> io::telemRx::transmitNTPStartMsg()
{
    return io::rtc::Time{};
}

std::expected<io::telemRx::RxChunk, ErrorCode> io::telemRx::read(std::span<uint8_t>)
{
    return RxChunk{ {}, io::rtc::Time{} };
}
