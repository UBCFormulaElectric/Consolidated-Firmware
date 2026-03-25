#include "io_telemUart.hpp"

#ifdef TARGET_EMBEDDED
#include "hw_uarts.hpp"

namespace io::telemUart
{

/**
 * Transmits an amount of data in polling mode (blocking).
 * @param tx_data Span of data to transmit.
 * @param timeout Timeout duration
 */
std::expected<void, ErrorCode> transmitPoll(std::span<const uint8_t> tx_data)
{
    return _900k_uart.transmit(tx_data);
}

/**
 * Receives an amount of data in interrupt mode (non-blocking).
 * @param tx_data Span of data to transmit.
 */
std::expected<void, ErrorCode> transmitIt(std::span<const uint8_t> tx_data)
{
    return _900k_uart.transmitIt(tx_data);
}

/**
 * Receives an amount of data in polling mode (blocking).
 * @param rx_data Span to store received data.
 * @param timeout Timeout duration
 */
std::expected<void, ErrorCode> receivePoll(std::span<uint8_t> rx_data)
{
    return _900k_uart.receivePoll(rx_data);
}

/**
 * Receives an amount of data in interrupt mode (non-blocking).
 * @param rx_data Span to store received data.
 */
std::expected<void, ErrorCode> receiveIt(std::span<uint8_t> rx_data)
{
    return _900k_uart.receiveIt(rx_data);
}

} // namespace io::telemUart
#endif
