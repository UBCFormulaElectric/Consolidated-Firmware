#pragma once

#include "util_errorCodes.hpp"
#include "hw_hal.hpp"
#include "FreeRTOS.h"
#include "task.h"

#include <span>

// HAL UART type documentation pg 1161:
// https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf

namespace hw
{
class Uart
{
#ifdef TARGET_EMBEDDED
  private:
    TaskHandle_t        taskInProgress;
    UART_HandleTypeDef &handle; // pointer to structure containing UART module configuration information
  public:
    explicit consteval Uart(UART_HandleTypeDef &in_handle) : taskInProgress(nullptr), handle(in_handle) {}
#endif
  private:
    /**
     * @param timeoutMs
     * @return
     */
    std::expected<void, ErrorCode> waitForNotification(uint32_t timeoutMs);

  public:
    /**
     *
     */
    void onTransactionCompleteFromISR() const;

    /**
     * Transmits an amount of data in polling mode (blocking).
     * @param tx Span of data to transmit.
     * @param timeout Timeout duration
     */
    std::expected<void, ErrorCode>
        transmit(std::span<const uint8_t> tx, uint32_t timeout = std::numeric_limits<uint32_t>::max());

    /**
     * Receives an amount of data in polling mode (blocking).
     * @param rx Span to store received data.
     * @param timeout Timeout duration
     */
    std::expected<void, ErrorCode>
        receive(std::span<uint8_t> rx, uint32_t timeout = std::numeric_limits<uint32_t>::max());

    // TODO
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

    void deinit() const;

    [[nodiscard]] const UART_HandleTypeDef &getHandle() const { return handle; }
};

[[nodiscard]] const Uart &getUartFromHandle(const UART_HandleTypeDef *handle);
} // namespace hw
