#include "io_telemRx.hpp"

#include <cstdint>
#include <span>

#include "hw_mutexGuard.hpp"
#include "hw_uarts.hpp"
#include "io_log.hpp"
#include "io_rtc.hpp"
#include "io_telemMessage.hpp"

// Send NTP request to backend and return the RTC time captured at send.
std::expected<io::rtc::Time, ErrorCode> io::telemRx::transmitNTPStartMsg()
{
    // Take the UART lock first so any wait for an in-flight transmit happens
    // *before* we capture t0.
    hw::MutexGuard g{ _900k_uart_tx_mutex };

    io::rtc::Time t0;
    const auto    t0_result = io::rtc::get_time(t0);
    if (!t0_result)
    {
        LOG_ERROR("Could not get RTC time");
        return std::unexpected(t0_result.error());
    }

    const io::telemMessage::NTPMsg ntp_msg{};
    const auto                     tx_result = _900k_uart.transmit(ntp_msg.asBytes(), 9);
    if (!tx_result)
    {
        LOG_ERROR("Failed to transmit NTP message");
        return std::unexpected(tx_result.error());
    }

    LOG_INFO(
        "Sent NTP Msg! at time: %02u:%02u:%02u.%03lu", t0.hours, t0.minutes, t0.seconds,
        static_cast<unsigned long>(999 - t0.subseconds));
    return t0;
}

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
