#include <cstring>
#include <cassert>

#include "cmsis_os2.h"
#include "hw_uart.hpp"
#include "hw_utils.hpp"

// constexpr size_t MAX_SPI_BUFFER = 256;

void hw::Uart::deinit() const
{
    HAL_UART_DeInit(&handle);
}

void hw::Uart::onTxTransactionCompleteFromISR() const
{
    assert(txTaskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(txTaskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw::Uart::onRxTransactionCompleteFromISR() const
{
    assert(rxTaskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(rxTaskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw::Uart::onErrorFromISR() const
{
    // just wake both up lmao
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    assert(txTaskInProgress != nullptr || rxTaskInProgress != nullptr);
    if (txTaskInProgress != nullptr)
    {
        vTaskNotifyGiveFromISR(txTaskInProgress, &higherPriorityTaskWoken);
        last_write_fault = true;
    }
    if (rxTaskInProgress != nullptr)
    {
        vTaskNotifyGiveFromISR(rxTaskInProgress, &higherPriorityTaskWoken);
        last_read_fault = true;
    }
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

std::expected<void, ErrorCode> hw::Uart::waitForRxNotification(const uint32_t timeoutMs) const
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
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

std::expected<void, ErrorCode> hw::Uart::waitForTxNotification(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    txTaskInProgress                 = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_AbortTransmit_IT(&handle);
        LOG_WARN("UART transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    if (last_write_fault)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

std::expected<void, ErrorCode> hw::Uart::transmit(const std::span<const uint8_t> tx, const uint32_t timeout) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const std::expected<void, ErrorCode> st =
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

    auto exit = utils::convertHalStatus(HAL_UART_Transmit_IT(&handle, tx.data(), static_cast<uint16_t>(tx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        txTaskInProgress = nullptr;
        return exit;
    }

    exit = waitForTxNotification(timeout);
    return exit;
}

std::expected<void, ErrorCode> hw::Uart::receive(std::span<uint8_t> rx, const uint32_t timeout) const
{
    if (rxTaskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const std::expected<void, ErrorCode> exit =
            utils::convertHalStatus(HAL_UART_Receive(&handle, rx.data(), static_cast<uint16_t>(rx.size()), timeout));
        return exit;
    }

    rxTaskInProgress = xTaskGetCurrentTaskHandle();
    last_read_fault  = false;

    std::expected<void, ErrorCode> exit =
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

std::expected<std::size_t, ErrorCode> hw::Uart::receiveToIdle(std::span<uint8_t> rx, const uint32_t timeout) const
{
    if (rxTaskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // Pre-kernel: blocking polling fallback. Not used in flight.
        uint16_t                             actual = 0;
        const std::expected<void, ErrorCode> exit   = utils::convertHalStatus(
            HAL_UARTEx_ReceiveToIdle(&handle, rx.data(), static_cast<uint16_t>(rx.size()), &actual, timeout));
        if (!exit)
            return std::unexpected(exit.error());
        return static_cast<std::size_t>(actual);
    }

    rxTaskInProgress = xTaskGetCurrentTaskHandle();
    last_read_fault  = false;
    last_rx_size     = 0;

    std::expected<void, ErrorCode> armed =
        utils::convertHalStatus(HAL_UARTEx_ReceiveToIdle_IT(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not armed.has_value())
    {
        rxTaskInProgress = nullptr;
        return std::unexpected(armed.error());
    }

    const std::expected<void, ErrorCode> waited = waitForRxNotification(timeout);
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

static const char *uart_error_to_name(const uint32_t err)
{
    switch (err)
    {
        case HAL_UART_ERROR_NONE:
            return "NONE";
        case HAL_UART_ERROR_PE:
            return "Parity Error";
        case HAL_UART_ERROR_NE:
            return "Noise Error";
        case HAL_UART_ERROR_FE:
            return "Frame Error";
        case HAL_UART_ERROR_ORE:
            return "Overrun Error";
        case HAL_UART_ERROR_DMA:
            return "DMA Error";
        default:
            return "UNKNOWN";
    }
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    LOG_ERROR("UART error: %s", uart_error_to_name(HAL_UART_GetError(huart)));

    // Clear sticky receive error flags so the next receive call isn't stuck on
    // ORE/NE/FE left over from this fault. Without this the peripheral keeps
    // signalling the same error and we stay deaf.
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);

    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onErrorFromISR();
}