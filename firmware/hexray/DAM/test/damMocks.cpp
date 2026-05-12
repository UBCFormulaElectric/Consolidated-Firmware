#include "io_canQueues.hpp"
#include "io_ntpButton.hpp"
#include "io_telemRx.hpp"
#include <expected>
#include <util_errorCodes.hpp>

io::queue<io::CanMsg, 128> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };
bool                       io::ntpButton::isPressed()
{
    return false;
}

std::expected<std::span<const uint8_t>, ErrorCode> io::telemRx::read(std::span<uint8_t>)
{
    return std::span<const uint8_t>{};
}
