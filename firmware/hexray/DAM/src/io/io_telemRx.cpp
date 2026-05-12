#include "io_telemRx.hpp"

#include <cstdint>
#include <span>

#include "hw_uarts.hpp"
#include "io_log.hpp"

std::expected<std::span<const uint8_t>, ErrorCode> io::telemRx::read(std::span<uint8_t> scratch)
{
    const auto rx_result = _900k_uart.receive(scratch);
    if (!rx_result)
    {
        LOG_ERROR("telemRx: UART receive failed: %d", static_cast<int>(rx_result.error()));
        return std::unexpected(ErrorCode::ERROR);
    }
    return std::span<const uint8_t>{ scratch };
}
