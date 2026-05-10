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
    mutable TaskHandle_t taskInProgress;
    UART_HandleTypeDef  &handle; // pointer to structure containing UART module configuration information
    bool                 callback_dma;
    mutable bool         rx_pending = false;
    void (*receive_callback)(void);

  public:
    explicit consteval Uart(
        UART_HandleTypeDef &in_handle,
        bool                in_callback_dma = false,
        void (*in_receive_callback)(void)   = nullptr)
      : taskInProgress(nullptr), handle(in_handle), callback_dma(in_callback_dma), receive_callback(in_receive_callback)
    {
    }

  private:
    /**
     * @param timeoutMs
     * @return
     */
    std::expected<void, ErrorCode> waitForNotification(uint32_t timeoutMs) const;

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
        transmit(std::span<const uint8_t> tx, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /**
     * Receives an amount of data in polling mode (blocking).
     * @param rx Span to store received data.
     * @param timeout Timeout duration
     */
    std::expected<void, ErrorCode>
        receive(std::span<uint8_t> rx, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /**
     * Receives an amount of data in non-blocking mode. Will fire the configured RX callback when complete.
     * @param rx Pointer to data buffer.
     */
    std::expected<void, ErrorCode> receiveCallback(std::span<uint8_t> rx) const;

    void deinit() const;

    [[nodiscard]] const UART_HandleTypeDef &getHandle() const { return handle; }
};
[[nodiscard]] const Uart &getUartFromHandle(const UART_HandleTypeDef *handle);
} // namespace hw
