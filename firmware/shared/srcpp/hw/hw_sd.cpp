#include "hw_sd.hpp"

#include "cmsis_os2.h"

#include <cstdint>

namespace hw
{
/* Setters for private fields */
void SdCard::onTransactionCompleteFromISR() const
{
    // A late completion can fire after waitForNotification() cleared taskInProgress on a timeout/abort;
    // only notify an actual waiter (mirrors onTransactionErrorFromISR()).
    if (taskInProgress == nullptr)
        return;

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void SdCard::onTransactionErrorFromISR() const
{
    // The SDMMC error ISR can fire outside of a DMA transaction (e.g. during init); only wake an actual waiter.
    if (taskInProgress == nullptr)
        return;

    transactionFailed = true;

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

result<void> SdCard::waitForNotification(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    taskInProgress                   = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        if (_hsd.State != HAL_SD_STATE_RESET)
            (void)HAL_SD_Abort_IT(&_hsd);
        LOG_WARN("SD transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    if (transactionFailed)
    {
        // Woken by the error ISR rather than a completion: the transfer failed (e.g. RX FIFO overrun).
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

result<void> SdCard::waitForCardReady() const
{
    if (!isDriverInitialized())
        return std::unexpected(ErrorCode::ERROR);

    // Bound the wait so a wedged/removed card can't hang the calling task forever.
    // The caller gets a TIMEOUT it can retry on, the same way the radio path does.
    const uint32_t start = HAL_GetTick();
    while (HAL_SD_GetCardState(&_hsd) != HAL_SD_CARD_TRANSFER)
    {
        if (HAL_GetTick() - start >= _timeout)
        {
            LOG_WARN("SD card not ready after %lu ms, giving up", static_cast<unsigned long>(_timeout));
            return std::unexpected(ErrorCode::TIMEOUT);
        }
        // Yield to equal/lower-priority tasks while waiting, but only once the scheduler
        // is running and we're not in an ISR — osDelay() is illegal otherwise.
        if (osKernelGetState() == taskSCHEDULER_RUNNING && not xPortIsInsideInterrupt())
            osDelay(1);
    }
    return {};
}

result<void> SdCard::read(std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    // The async path feeds this pointer to the SDMMC internal DMA, which requires a 4-byte-aligned base.
    if (reinterpret_cast<uintptr_t>(pdata.data()) % 4 != 0)
        return std::unexpected(ErrorCode::INVALID_ARGS);

    CHECK_SD_PRESENT();
    if (const auto ready = waitForCardReady(); not ready)
        return ready;

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const result<void> st = utils::convertHalStatus(
            HAL_SD_ReadBlocks(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, _timeout));
        return st;
    }

    if (taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SD transaction.
    taskInProgress    = xTaskGetCurrentTaskHandle();
    transactionFailed = false;

    result<void> exit = utils::convertHalStatus(
        HAL_SD_ReadBlocks_DMA(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification(_timeout);
    return exit;
}

result<void> SdCard::readOffset(const std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset) const
{
    CHECK_SD_PRESENT();
    if (not OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return std::unexpected(ErrorCode::ERROR);
    return read(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
}

result<void> SdCard::write(const std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    // The async path feeds this pointer to the SDMMC internal DMA, which requires a 4-byte-aligned base.
    if (reinterpret_cast<uintptr_t>(pdata.data()) % 4 != 0)
        return std::unexpected(ErrorCode::INVALID_ARGS);

    CHECK_SD_PRESENT();
    if (const auto ready = waitForCardReady(); not ready)
        return ready;

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const result<void> st = utils::convertHalStatus(
            HAL_SD_WriteBlocks(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, _timeout));
        return st;
    }

    if (taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SD transaction.
    taskInProgress    = xTaskGetCurrentTaskHandle();
    transactionFailed = false;

    result<void> exit = utils::convertHalStatus(
        HAL_SD_WriteBlocks_DMA(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification(_timeout);
    return exit;
}

result<void> SdCard::writeOffset(const std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset) const
{
    if (pdata.empty())
        return {};

    CHECK_SD_PRESENT();
    if (not OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return std::unexpected(ErrorCode::ERROR);
    return write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
}

result<void> SdCard::erase(const uint32_t start_addr, const uint32_t end_addr) const
{
    CHECK_SD_PRESENT();
    if (const auto ready = waitForCardReady(); not ready)
        return ready;
    // TODO I think this needs to be a tx?
    return hw::utils::convertHalStatus(HAL_SD_Erase(&_hsd, start_addr, end_addr));
}

result<void> SdCard::writeDma(const std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    CHECK_SD_PRESENT();
    UNUSED(block_addr);
    return {};
    // TODO
    // while (!dma_tx_completed)
    //     ;
    // while (HAL_SD_GetCardState(_hsd) != HAL_SD_CARD_TRANSFER)
    //     ;
    //
    // dma_tx_completed = false;
    // return hw::utils::convertHalStatus(
    //     HAL_SD_WriteBlocks_DMA(_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
}
} // namespace hw

/* HAL callbacks are not inside hw namespace */
extern "C"
{
    void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.onTransactionCompleteFromISR();
    }

    void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.onTransactionCompleteFromISR();
    }

    void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        LOG_ERROR("error: 0x%lX, %s", HAL_SD_GetError(&sd.getHsd()), sd.getErrorString());
        // Wake the blocked task with a failure instead of letting it wait out the full timeout.
        sd.onTransactionErrorFromISR();
    }

    void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        // TODO set error
        LOG_INFO("aborted");
    }
} // extern C
