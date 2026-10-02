#include "io_telemRx.hpp"

#include <cstdint>
#include <span>

#include "hw_uarts.hpp"
result<std::span<const uint8_t>> io::telemRx::read(std::span<uint8_t> scratch)
{
    // ReceiveToIdle keeps the peripheral armed for the whole scratch buffer in
    // a single HAL call, so there's no inter-byte re-arm gap that can overrun.
    // Returns whatever arrived before the line idled, the buffer filled, or
    // the half-buffer mark was hit.
    const auto rx_result = _900k_uart.receiveToIdle(scratch);
    if (!rx_result)
        return std::unexpected(rx_result.error());
    return std::span<const uint8_t>{ scratch.data(), *rx_result };
}
