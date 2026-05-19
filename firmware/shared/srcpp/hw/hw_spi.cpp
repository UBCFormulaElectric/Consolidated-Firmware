#include <cassert>
#include <cstring>
#include "cmsis_os2.h"
#include "hw_spi.hpp"
#include "hw_utils.hpp"

using namespace hw::spi;

constexpr size_t MAX_SPI_BUFFER = 256;

/* ------------------------------- SpiBus ------------------------------- */
void bus::deinit() const
{
    HAL_SPI_DeInit(&handle);
}

void bus::onTransactionCompleteFromISR() const
{
    assert(taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

/* ------------------------------ SpiDevice ----------------------------- */
void device::enableNss() const
{
    if (nss.has_value())
    {
        nss.value().writePin(false);
    }
}

void device::disableNss() const
{
    if (nss.has_value())
    {
        nss.value().writePin(true);
    }
}

result<void> device::waitForNotification() const
{
    // Block the calling task until the SPI ISR wakes it (success or error), or until timeoutMs expires.
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    parent_bus.taskInProgress        = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // No notification arrived within the timeout: tear down the in-flight transfer.
        (void)HAL_SPI_Abort_IT(&parent_bus.handle);
        LOG_WARN("SPI transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    // We were woken, but check whether HAL flagged an error mid-transfer (e.g. overrun, mode fault).
    // The error callback also calls onTransactionCompleteFromISR, so we land here on errors too.
    if (parent_bus.handle.ErrorCode != HAL_SPI_ERROR_NONE)
    {
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

result<void> device::transmit(std::span<const uint8_t> tx) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss();
        const result<void> st = hw::utils::convertHalStatus(HAL_SPI_Transmit(
            &parent_bus.handle, const_cast<uint8_t *>(tx.data()), static_cast<uint16_t>(tx.size()), timeoutMs));
        disableNss();
        return st;
    }

    if (parent_bus.taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SPI transaction.
    parent_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    enableNss();

    auto exit = hw::utils::convertHalStatus(
        HAL_SPI_Transmit_IT(&parent_bus.handle, const_cast<uint8_t *>(tx.data()), static_cast<uint16_t>(tx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        parent_bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    return exit;
}

result<void> device::receive(std::span<uint8_t> rx) const
{
    if (parent_bus.taskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss();
        const result<void> exit = hw::utils::convertHalStatus(
            HAL_SPI_Receive(&parent_bus.handle, rx.data(), static_cast<uint16_t>(rx.size()), timeoutMs));
        disableNss();
        return exit;
    }

    parent_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    enableNss();

    result<void> exit = hw::utils::convertHalStatus(
        HAL_SPI_Receive_IT(&parent_bus.handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        parent_bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    return exit;
}

[[nodiscard]] result<void> device::transmitThenReceive(const std::span<const uint8_t> tx, std::span<uint8_t> rx) const
{
    const auto combined = static_cast<uint16_t>(tx.size() + rx.size());
    if (combined > MAX_SPI_BUFFER)
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    // HAL_SPI_TransmitReceive_IT requires tx buffer and rx buffer to be of size equal or greater to number of bytes to
    // transmit and receive
    uint8_t paddedTx[MAX_SPI_BUFFER];
    uint8_t paddedRx[MAX_SPI_BUFFER];
    std::memset(paddedTx, 0, combined);
    std::memset(paddedRx, 0, combined);

    // Copy tx_buffer into beginning of larger padded_tx_buffer
    std::memcpy(paddedTx, tx.data(), tx.size());

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss();
        const result<void> exit = hw::utils::convertHalStatus(
            HAL_SPI_TransmitReceive(&parent_bus.handle, paddedTx, paddedRx, combined, timeoutMs));
        disableNss();

        // Data will not be returned over SPI until command has finished, so data in first tx_buffer_size bytes not
        // relevant Copy entries at the end of padded_rx_buffer back into rx_buffer
        std::memcpy(rx.data(), paddedRx + tx.size(), rx.size());

        return exit;
    }

    if (parent_bus.taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SPI transaction.
    parent_bus.taskInProgress = xTaskGetCurrentTaskHandle();
    enableNss();

    result<void> exit =
        hw::utils::convertHalStatus(HAL_SPI_TransmitReceive_IT(&parent_bus.handle, paddedTx, paddedRx, combined));
    if (not exit.has_value())
    {
        parent_bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    if (not exit)
    {
        return exit;
    }

    /* Data will not be returned over SPI until command has finished, so data in first tx_buffer_size bytes not relevant
    Copy entries at the end of padded_rx_buffer back into rx_buffer */
    std::memcpy(rx.data(), paddedRx + tx.size(), rx.size());

    return {};
}

/* ------------------------------ DMA methods ----------------------------- */
// These mirror transmit/receive/transmitThenReceive but use HAL_SPI_*_DMA under the hood. The TX path
// stages caller data through dma_tx_buf, RX path copies out of dma_rx_buf after completion. The bus's
// taskInProgress handle still serializes all transfers across all DMA/IT methods on the same bus.

std::expected<void, ErrorCode> device::transmitDma(const std::span<const uint8_t> tx) const
{
    if (tx.size() > sizeof(dma_tx_buf))
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // No scheduler / inside ISR: can't block on a notification, fall back to polled HAL.
        enableNss();
        const auto st = hw::utils::convertHalStatus(HAL_SPI_Transmit(
            &parent_bus.handle, const_cast<uint8_t *>(tx.data()), static_cast<uint16_t>(tx.size()), timeoutMs));
        disableNss();
        return st;
    }

    if (parent_bus.taskInProgress != nullptr)
    {
        // Another task already owns the bus.
        return std::unexpected(ErrorCode::BUSY);
    }

    // Claim the bus before any HAL call so the completion ISR knows who to wake.
    parent_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    // Stage TX into the device-owned buffer so DMA reads from a known location.
    std::memcpy(dma_tx_buf, tx.data(), tx.size());

    enableNss();
    auto exit = hw::utils::convertHalStatus(
        HAL_SPI_Transmit_DMA(&parent_bus.handle, dma_tx_buf, static_cast<uint16_t>(tx.size())));
    if (not exit.has_value())
    {
        // DMA kickoff failed: release the bus and bail.
        parent_bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    return exit;
}

std::expected<void, ErrorCode> device::receiveDma(const std::span<uint8_t> rx) const
{
    if (rx.size() > sizeof(dma_rx_buf))
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // No scheduler / inside ISR: can't block on a notification, fall back to polled HAL.
        enableNss();
        const auto exit = hw::utils::convertHalStatus(
            HAL_SPI_Receive(&parent_bus.handle, rx.data(), static_cast<uint16_t>(rx.size()), timeoutMs));
        disableNss();
        return exit;
    }

    if (parent_bus.taskInProgress != nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    parent_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    enableNss();
    auto exit = hw::utils::convertHalStatus(
        HAL_SPI_Receive_DMA(&parent_bus.handle, dma_rx_buf, static_cast<uint16_t>(rx.size())));
    if (not exit.has_value())
    {
        parent_bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    if (exit)
    {
        // DMA wrote into dma_rx_buf; surface it to the caller's span.
        std::memcpy(rx.data(), dma_rx_buf, rx.size());
    }
    return exit;
}

[[nodiscard]] std::expected<void, ErrorCode>
    device::transmitThenReceiveDma(const std::span<const uint8_t> tx, const std::span<uint8_t> rx) const
{
    // SPI is full-duplex: to clock in rx.size() bytes we must clock out the same number. Build a single
    // TX frame = [cmd bytes][zero-padded dummy bytes for the read phase].
    const auto combined = static_cast<uint16_t>(tx.size() + rx.size());
    if (combined > MAX_SPI_BUFFER)
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }

    std::memset(dma_tx_buf, 0, combined);
    std::memcpy(dma_tx_buf, tx.data(), tx.size());

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // No scheduler / inside ISR: can't block on a notification, fall back to polled HAL.
        enableNss();
        const auto exit = hw::utils::convertHalStatus(
            HAL_SPI_TransmitReceive(&parent_bus.handle, dma_tx_buf, dma_rx_buf, combined, timeoutMs));
        disableNss();

        // The first tx.size() RX bytes are clocked in during the command phase (garbage from the slave's
        // perspective). The real response lives at offset tx.size().
        std::memcpy(rx.data(), dma_rx_buf + tx.size(), rx.size());
        return exit;
    }

    if (parent_bus.taskInProgress != nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    parent_bus.taskInProgress = xTaskGetCurrentTaskHandle();

    enableNss();
    auto exit = hw::utils::convertHalStatus(
        HAL_SPI_TransmitReceive_DMA(&parent_bus.handle, dma_tx_buf, dma_rx_buf, combined));
    if (not exit.has_value())
    {
        parent_bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    if (not exit)
    {
        return exit;
    }

    // Slice out the response, discarding the leading command-phase bytes.
    std::memcpy(rx.data(), dma_rx_buf + tx.size(), rx.size());
    return {};
}

/* ---------------------------- HAL callbacks --------------------------- */
// HAL fires the same TxCplt/RxCplt/TxRxCplt callbacks for both IT and DMA modes, so a single set of
// callbacks handles both. Each wakes the task currently blocked in waitForNotification.

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    const auto &bus = getBusFromHandle(hspi);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    const auto &bus = getBusFromHandle(hspi);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    const auto &bus = getBusFromHandle(hspi);
    bus.onTransactionCompleteFromISR();
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    LOG_ERROR("SPI error: 0x%X", hspi->ErrorCode);
    // Wake the waiting task immediately instead of letting it block for the full timeout.
    // waitForNotification inspects hspi->ErrorCode and converts the wake into ErrorCode::ERROR.
    const auto &bus = getBusFromHandle(hspi);
    bus.onTransactionCompleteFromISR();
}
