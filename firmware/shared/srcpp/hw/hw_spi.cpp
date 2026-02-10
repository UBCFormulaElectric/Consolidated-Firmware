#include <cassert>
#include <cstring>
#include "cmsis_os2.h"
#include "hw_spi.hpp"
#include "hw_utils.hpp"

using namespace hw::spi;

constexpr size_t MAX_SPI_BUFFER = 256;

/* ------------------------------- SpiBus ------------------------------- */
void SpiBus::deinit() const
{
    HAL_SPI_DeInit(&handle);
}

void SpiBus::onTransactionCompleteFromISR() const
{
    assert(taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

/* ------------------------------ SpiDevice ----------------------------- */
void SpiDevice::enableNss() const
{
    nss.writePin(false);
}

void SpiDevice::disableNss() const
{
    nss.writePin(true);
}

std::expected<void, ErrorCode> SpiDevice::waitForNotification() const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    bus.taskInProgress               = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_SPI_Abort_IT(&bus.handle);
        LOG_WARN("SPI transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    return {};
}

std::expected<void, ErrorCode> SpiDevice::transmit(const std::span<const uint8_t> tx) const
{
    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss();
        const std::expected<void, ErrorCode> st = hw_utils_convertHalStatus(
            HAL_SPI_Transmit(&bus.handle, tx.data(), static_cast<uint16_t>(tx.size()), timeoutMs));
        disableNss();
        return st;
    }

    if (bus.taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SPI transaction.
    bus.taskInProgress = xTaskGetCurrentTaskHandle();

    enableNss();

    auto exit =
        hw_utils_convertHalStatus(HAL_SPI_Transmit_IT(&bus.handle, tx.data(), static_cast<uint16_t>(tx.size())));
    if (not exit)
    {
        // Mark this transaction as no longer in progress.
        bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    return exit;
}

std::expected<void, ErrorCode> SpiDevice::receive(const std::span<uint8_t> rx) const
{
    if (bus.taskInProgress != nullptr || xPortIsInsideInterrupt())
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    if (osKernelGetState() != taskSCHEDULER_RUNNING)
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        enableNss();
        const std::expected<void, ErrorCode> exit = hw_utils_convertHalStatus(
            HAL_SPI_Receive(&bus.handle, rx.data(), static_cast<uint16_t>(rx.size()), timeoutMs));
        disableNss();
        return exit;
    }

    bus.taskInProgress = xTaskGetCurrentTaskHandle();

    enableNss();

    std::expected<void, ErrorCode> exit =
        hw_utils_convertHalStatus(HAL_SPI_Receive_IT(&bus.handle, rx.data(), static_cast<uint16_t>(rx.size())));
    if (not exit)
    {
        // Mark this transaction as no longer in progress.
        bus.taskInProgress = nullptr;
        disableNss();
        return exit;
    }

    exit = waitForNotification();
    disableNss();
    return exit;
}

[[nodiscard]] std::expected<void, ErrorCode>
    SpiDevice::transmitThenReceive(const std::span<const uint8_t> tx, const std::span<uint8_t> rx) const
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
        const std::expected<void, ErrorCode> exit =
            hw_utils_convertHalStatus(HAL_SPI_TransmitReceive(&bus.handle, paddedTx, paddedRx, combined, timeoutMs));
        disableNss();

        // Data will not be returned over SPI until command has finished, so data in first tx_buffer_size bytes not
        // relevant Copy entries at the end of padded_rx_buffer back into rx_buffer
        std::memcpy(rx.data(), paddedRx + tx.size(), rx.size());

        return exit;
    }

    if (bus.taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SPI transaction.
    bus.taskInProgress = xTaskGetCurrentTaskHandle();
    enableNss();

    std::expected<void, ErrorCode> exit =
        hw_utils_convertHalStatus(HAL_SPI_TransmitReceive_IT(&bus.handle, paddedTx, paddedRx, combined));
    if (not exit)
    {
        bus.taskInProgress = nullptr;
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

/* ---------------------------- HAL callbacks --------------------------- */
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
}
