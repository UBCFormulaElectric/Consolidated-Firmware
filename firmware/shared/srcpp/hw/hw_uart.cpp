#include <cstring>
#include <cassert>

#include "cmsis_os2.h"
#include "hw_uart.hpp"
#include "hw_utils.hpp"

// constexpr size_t MAX_SPI_BUFFER = 256;

static void clear_uart_rx_error_flags(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);
}

void hw::Uart::deinit() const
{
    HAL_UART_DeInit(&handle);
}

void hw::Uart::onTxTransactionCompleteFromISR() const
{
    if (txTaskInProgress == nullptr)
        return;

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(txTaskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw::Uart::onRxTransactionCompleteFromISR() const
{
    if (rxTaskInProgress == nullptr)
        return;

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(rxTaskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw::Uart::onErrorFromISR(const uint32_t hal_error) const
{
    BaseType_t         higherPriorityTaskWoken = pdFALSE;
    constexpr uint32_t rx_error_flags = HAL_UART_ERROR_PE | HAL_UART_ERROR_NE | HAL_UART_ERROR_FE | HAL_UART_ERROR_ORE;
    const bool         rx_fault       = (hal_error & rx_error_flags) != 0U;
    const bool         dma_fault      = (hal_error & HAL_UART_ERROR_DMA) != 0U;

    if (txTaskInProgress != nullptr && (dma_fault || !rx_fault))
    {
        vTaskNotifyGiveFromISR(txTaskInProgress, &higherPriorityTaskWoken);
        last_write_fault = true;
    }
    if (rxTaskInProgress != nullptr && (rx_fault || dma_fault || hal_error == HAL_UART_ERROR_NONE))
    {
        vTaskNotifyGiveFromISR(rxTaskInProgress, &higherPriorityTaskWoken);
        last_read_fault = true;
    }
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

result<void> hw::Uart::waitForRxNotification(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    rxTaskInProgress                 = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_AbortReceive_IT(&handle);
        LOG_WARN("UART transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    if (last_read_fault)
    {
        (void)HAL_UART_AbortReceive(&handle);
        clear_uart_rx_error_flags(&handle);
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

result<void> hw::Uart::waitForTxNotification(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    txTaskInProgress                 = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        if (handle.hdmatx != nullptr)
            (void)HAL_UART_AbortTransmit(&handle);
        else
            (void)HAL_UART_AbortTransmit_IT(&handle);
        LOG_WARN("UART transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    if (last_write_fault)
    {
        txTaskInProgress = nullptr;
        return std::unexpected(ErrorCode::ERROR);
    }
    txTaskInProgress = nullptr;
    return {};
}

result<void> hw::Uart::transmit(const std::span<const uint8_t> tx, const uint32_t timeout) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const result<void> st =
            utils::convertHalStatus(HAL_UART_Transmit(&handle, tx.data(), static_cast<uint16_t>(tx.size()), timeout));
        return st;
    }

    if (txTaskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a UART transaction.
    txTaskInProgress = xTaskGetCurrentTaskHandle();
    last_write_fault = false;

    // Use DMA when a TX DMA channel is linked for this UART (configured in the
    // board's CubeMX MSP); otherwise fall back to interrupt-driven TX. DMA keeps
    // the per-frame ISR cost flat as the baud rate scales, instead of taking a
    // FIFO interrupt every few bytes.
    auto exit =
        (handle.hdmatx != nullptr)
            ? utils::convertHalStatus(HAL_UART_Transmit_DMA(&handle, tx.data(), static_cast<uint16_t>(tx.size())))
            : utils::convertHalStatus(HAL_UART_Transmit_IT(&handle, tx.data(), static_cast<uint16_t>(tx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        txTaskInProgress = nullptr;
        return exit;
    }

    exit = waitForTxNotification(timeout);
    return exit;
}

result<void> hw::Uart::receive(std::span<uint8_t> rx, const uint32_t timeout) const
{
    if (rxTaskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const result<void> exit =
            utils::convertHalStatus(HAL_UART_Receive(&handle, rx.data(), static_cast<uint16_t>(rx.size()), timeout));
        return exit;
    }

    rxTaskInProgress = xTaskGetCurrentTaskHandle();
    last_read_fault  = false;

    result<void> exit =
        utils::convertHalStatus(HAL_UART_Receive_IT(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        rxTaskInProgress = nullptr;
        return exit;
    }

    exit = waitForRxNotification(timeout);
    return exit;
}

result<std::size_t> hw::Uart::receiveToIdle(std::span<uint8_t> rx, const uint32_t timeout) const
{
    if (rxTaskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // Pre-kernel: blocking polling fallback. Not used in flight.
        uint16_t           actual = 0;
        const result<void> exit   = utils::convertHalStatus(
            HAL_UARTEx_ReceiveToIdle(&handle, rx.data(), static_cast<uint16_t>(rx.size()), &actual, timeout));
        if (!exit)
            return std::unexpected(exit.error());
        return static_cast<std::size_t>(actual);
    }

    rxTaskInProgress = xTaskGetCurrentTaskHandle();
    last_read_fault  = false;
    last_rx_size     = 0;

    result<void> armed =
        utils::convertHalStatus(HAL_UARTEx_ReceiveToIdle_IT(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not armed.has_value())
    {
        (void)HAL_UART_AbortReceive(&handle);
        clear_uart_rx_error_flags(&handle);
        rxTaskInProgress = nullptr;
        return std::unexpected(armed.error());
    }

    // waitForRxNotification() always clears rxTaskInProgress before returning, on every path.
    const result<void> waited = waitForRxNotification(timeout);
    if (!waited)
        return std::unexpected(waited.error());

    return static_cast<std::size_t>(last_rx_size);
}

/* ---------------------------- HAL callbacks --------------------------- */
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onRxTransactionCompleteFromISR();
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    // Fires on idle line, half-buffer, or full-buffer when armed via
    // HAL_UARTEx_ReceiveToIdle_IT/DMA. `size` is the total bytes written into
    // the rx buffer so far for this transaction.
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.setLastRxSizeFromISR(size);
    bus.onRxTransactionCompleteFromISR();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onTxTransactionCompleteFromISR();
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    const uint32_t hal_error = HAL_UART_GetError(huart);

    // Clear sticky receive error flags so the next receive call isn't stuck on
    // ORE/NE/FE left over from this fault. Without this the peripheral keeps
    // signalling the same error and we stay deaf.
    clear_uart_rx_error_flags(huart);

    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onErrorFromISR(hal_error);
}
