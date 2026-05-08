#include "io_telemRx.hpp"

#include <cstdint>
#include <span>

#include "hw_uarts.hpp"
#include "io_log.hpp"
#include "io_rtc.hpp"

// TODO: replace blocking chunk read with UART-IDLE / DMA driven ingestion so
// rx_time is captured at the byte-arrival instant rather than at chunk end.
std::expected<io::telemRx::RxChunk, ErrorCode> io::telemRx::read(std::span<uint8_t> scratch)
{
    const auto rx_result = _900k_uart.receive(scratch);
    if (!rx_result)
    {
        LOG_ERROR("telemRx: UART receive failed: %d", static_cast<int>(rx_result.error()));
        return std::unexpected(ErrorCode::ERROR);
    }

    io::rtc::Time t3{};
    const auto    t3_result = io::rtc::get_time(t3);
    if (!t3_result)
    {
        LOG_ERROR("telemRx: could not capture t3");
        return std::unexpected(t3_result.error());
    }

    return RxChunk{ std::span<const uint8_t>{ scratch }, t3 };
}
