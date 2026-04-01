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

void hw::Uart::onTransactionCompleteFromISR_Rx() const
{
    assert(Rx_taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(Rx_taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw::Uart::onErrorFromISR_Rx() const
{
    assert(Rx_taskInProgress != nullptr);
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(Rx_taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);

    last_read_fault = true;
}

std::expected<void, ErrorCode> hw::Uart::waitForNotification_Rx(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    Rx_taskInProgress                   = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_Abort_IT(&handle);
        LOG_WARN("UART transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    if (last_read_fault)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

void hw::Uart::onTransactionCompleteFromISR_Tx() const
{
    assert(Tx_taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(Tx_taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void hw::Uart::onErrorFromISR_Tx() const
{
    assert(Tx_taskInProgress != nullptr);
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(Tx_taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);

    last_read_fault = true;
}

std::expected<void, ErrorCode> hw::Uart::waitForNotification_Tx(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    Tx_taskInProgress                   = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_Abort_IT(&handle);
        LOG_WARN("UART transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    if (last_read_fault)
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
            hw_utils_convertHalStatus(HAL_UART_Transmit(&handle, tx.data(), static_cast<uint16_t>(tx.size()), timeout));
        return st;
    }

    if (Tx_taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a UART transaction.
    Tx_taskInProgress = xTaskGetCurrentTaskHandle();

    auto exit = hw_utils_convertHalStatus(HAL_UART_Transmit_IT(&handle, tx.data(), static_cast<uint16_t>(tx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        Tx_taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification_Tx(timeout);
    return exit;
}

std::expected<void, ErrorCode> hw::Uart::receive(std::span<uint8_t> rx, const uint32_t timeout) const
{
    if (Rx_taskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const std::expected<void, ErrorCode> exit =
            hw_utils_convertHalStatus(HAL_UART_Receive(&handle, rx.data(), static_cast<uint16_t>(rx.size()), timeout));
        return exit;
    }

    Rx_taskInProgress  = xTaskGetCurrentTaskHandle();
    last_read_fault = false;

    std::expected<void, ErrorCode> exit =
        hw_utils_convertHalStatus(HAL_UART_Receive_IT(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        Rx_taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification_Rx(timeout);
    return exit;
}

/* ---------------------------- HAL callbacks --------------------------- */
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onTransactionCompleteFromISR_Rx();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onTransactionCompleteFromISR_Tx();
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    LOG_ERROR("UART error: 0x%X", huart->ErrorCode);
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onErrorFromISR_Rx();
    bus.onErrorFromISR_Tx();
}
