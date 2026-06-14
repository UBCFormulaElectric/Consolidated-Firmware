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
  private:
    mutable TaskHandle_t rxTaskInProgress = nullptr;
    mutable TaskHandle_t txTaskInProgress = nullptr;

    UART_HandleTypeDef &handle; // pointer to structure containing UART module configuration information
  public:
    explicit consteval Uart(UART_HandleTypeDef &in_handle) : handle(in_handle) {}

  private:
    /**
     * @param timeoutMs
     * @return
     */
    result<void> waitForTxNotification(uint32_t timeoutMs) const;
    result<void> waitForRxNotification(uint32_t timeoutMs) const;

    mutable bool last_read_fault  = false;
    mutable bool last_write_fault = false;

    // Bytes delivered by the most recent HAL_UARTEx_RxEventCallback (idle/half/full).
    // Written from ISR, read by the task after it wakes from waitForRxNotification.
    // No `volatile` needed: the FreeRTOS task-notify pair (vTaskNotifyGiveFromISR ->
    // ulTaskNotifyTake) provides the memory barrier between writer and reader.
    // `volatile` would also break the class's literal-type requirement for constexpr.
    mutable uint16_t last_rx_size = 0;

  public:
    /**
     *
     */
    void onTxTransactionCompleteFromISR() const;
    void onRxTransactionCompleteFromISR() const;

    void onErrorFromISR(uint32_t hal_error) const;

    /**
     * Transmits an amount of data in polling mode (blocking).
     * @param tx Span of data to transmit.
     * @param timeout Timeout duration
     */
    result<void> transmit(std::span<const uint8_t> tx, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /**
     * Receives an amount of data in polling mode (blocking).
     * @param rx Span to store received data.
     * @param timeout Timeout duration
     */
    result<void> receive(std::span<uint8_t> rx, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /**
     * Receives up to rx.size() bytes using HAL_UARTEx_ReceiveToIdle_IT. Returns
     * the number of bytes actually received when the line idles, the buffer
     * fills, or the half-buffer point is hit — whichever comes first. Unlike
     * receive(), the peripheral is armed for the whole buffer in a single HAL
     * call, so there is no inter-byte re-arm gap that can cause an overrun.
     * @param rx Span to store received data.
     * @param timeout Timeout duration.
     */
    result<std::size_t>
        receiveToIdle(std::span<uint8_t> rx, uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;

    /// Bytes carried out of the RxEvent ISR for the last receiveToIdle() call.
    [[nodiscard]] uint16_t lastRxSize() const { return last_rx_size; }
    void                   setLastRxSizeFromISR(uint16_t size) const { last_rx_size = size; }

    void deinit() const;

    [[nodiscard]] const UART_HandleTypeDef &getHandle() const { return handle; }
};
[[nodiscard]] const Uart &getUartFromHandle(const UART_HandleTypeDef *handle);
} // namespace hw
