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

void hw::Uart::onTransactionCompleteFromISR() const
{
    if (rx_pending)
    {
        assert(receive_callback != nullptr);
        receive_callback();
        return;
    }

    assert(taskInProgress != nullptr);
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

std::expected<void, ErrorCode> hw::Uart::waitForNotification(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    taskInProgress                   = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_UART_Abort_IT(&handle);
        LOG_WARN("UART transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
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

    if (taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a UART transaction.
    taskInProgress = xTaskGetCurrentTaskHandle();

    auto exit = utils::convertHalStatus(HAL_UART_Transmit_IT(&handle, tx.data(), static_cast<uint16_t>(tx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification(timeout);
    return exit;
}

std::expected<void, ErrorCode> hw::Uart::receive(std::span<uint8_t> rx, const uint32_t timeout) const
{
    if (taskInProgress != nullptr || xPortIsInsideInterrupt())
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

    taskInProgress = xTaskGetCurrentTaskHandle();

    std::expected<void, ErrorCode> exit =
        utils::convertHalStatus(HAL_UART_Receive_IT(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification(timeout);
    return exit;
}

/**
 * Receives an amount of data in non-blocking mode. Will fire the configured RX callback when complete.
 * @param rx Pointer to data buffer.
 */
std::expected<void, ErrorCode> hw::Uart::receiveCallback(std::span<uint8_t> rx) const
{
    assert(receive_callback != nullptr);

    if (taskInProgress != nullptr)
        return std::unexpected(ErrorCode::BUSY); // There is a task currently in progress!

    rx_pending = true;

    if (callback_dma)
        return hw::utils::convertHalStatus(HAL_UART_Receive_DMA(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
    else
        return hw::utils::convertHalStatus(HAL_UART_Receive_IT(&handle, rx.data(), static_cast<uint16_t>(rx.size())));
}

/* ---------------------------- HAL callbacks --------------------------- */
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    const hw::Uart &bus = hw::getUartFromHandle(huart);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    LOG_ERROR("UART error: 0x%X", huart->ErrorCode);
}
