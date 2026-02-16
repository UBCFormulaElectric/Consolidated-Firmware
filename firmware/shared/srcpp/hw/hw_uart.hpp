#pragma once

#include "util_errorCodes.hpp"
#include <span>

#ifdef TARGET_EMBEDDED
#include "hw_hal.hpp"
#endif

// HAL UART type documentation pg 1161:
// https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf

namespace hw
{
class Uart
{
#ifdef TARGET_EMBEDDED
  private:
    UART_HandleTypeDef *const handle; // pointer to structure containing UART module configuration information
  public:
    explicit consteval Uart(UART_HandleTypeDef *in_handle) : handle(in_handle) {}
#endif
  public:
    /**
     * Transmits an amount of data in DMA mode (non-blocking).
     * @param tx_data Span of data to transmit.
     */
    std::expected<void, ErrorCode> transmitDma(std::span<const uint8_t> tx_data) const;

    /**
     * Receives an amount of data in DMA mode (non-blocking).
     * @param rx_data Span to store received data.
     */
    std::expected<void, ErrorCode> receiveDma(std::span<uint8_t> rx_data) const;

    /**
     * Transmits an amount of data in polling mode (blocking).
     * @param tx_data Span of data to transmit.
     * @param timeout Timeout duration
     */
    std::expected<void, ErrorCode>
        transmitPoll(std::span<const uint8_t> tx_data, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /**
     * Receives an amount of data in polling mode (blocking).
     * @param rx_data Span to store received data.
     * @param timeout Timeout duration
     */
    std::expected<void, ErrorCode>
        receivePoll(std::span<uint8_t> rx_data, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /**
     * Receives an amount of data in interrupt mode (non-blocking).
     * @param tx_data Span of data to transmit.
     */
    std::expected<void, ErrorCode> transmitIt(std::span<const uint8_t> tx_data) const;

    /**
     * Receives an amount of data in interrupt mode (non-blocking).
     * @param rx_data Span to store received data.
     */
    std::expected<void, ErrorCode> receiveIt(std::span<uint8_t> rx_data) const;

    [[nodiscard]] const UART_HandleTypeDef *getHandle() const { return handle; }
};

[[nodiscard]] const Uart &getUartFromHandle(const UART_HandleTypeDef *handle);
} // namespace hw
