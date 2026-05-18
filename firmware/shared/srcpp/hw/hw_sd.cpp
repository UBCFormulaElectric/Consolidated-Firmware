#include "hw_sd.hpp"

#include "cmsis_os2.h"

#include <cassert>
#include <cstdint>

namespace hw
{
/* Setters for private fields */
void SdCard::onTransactionCompleteFromISR() const
{
    // dma_tx_completed = value;
    // TODO signaling to blocked here
    assert(taskInProgress != nullptr);

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(taskInProgress, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

std::expected<void, ErrorCode> SdCard::waitForNotification(const uint32_t timeoutMs) const
{
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, timeoutMs);
    taskInProgress                   = nullptr;

    if (const bool transaction_timed_out = num_notifications; transaction_timed_out == 0)
    {
        // If the transaction didn't complete within the timeout, manually abort it.
        (void)HAL_SD_Abort_IT(&_hsd);
        LOG_WARN("SD transaction timed out");
        return std::unexpected(ErrorCode::TIMEOUT);
    }
    return {};
}

std::expected<void, ErrorCode> SdCard::read(std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(&_hsd) != HAL_SD_CARD_TRANSFER)
        ;

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const std::expected<void, ErrorCode> st = utils::convertHalStatus(
            HAL_SD_ReadBlocks(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, _timeout));
        return st;
    }

    if (taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SD transaction.
    taskInProgress = xTaskGetCurrentTaskHandle();

    std::expected<void, ErrorCode> exit = utils::convertHalStatus(
        HAL_SD_ReadBlocks_IT(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification(_timeout);
    return exit;
}

std::expected<void, ErrorCode>
    SdCard::readOffset(const std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset) const
{
    CHECK_SD_PRESENT();
    if (not OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return std::unexpected(ErrorCode::ERROR);
    return read(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
}

std::expected<void, ErrorCode> SdCard::write(const std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    // TODO this on interrupt basis as well?
    while (HAL_SD_GetCardState(&_hsd) != HAL_SD_CARD_TRANSFER)
        ;

    if (osKernelGetState() != taskSCHEDULER_RUNNING || xPortIsInsideInterrupt())
    {
        // If kernel hasn't started, there's no current task to block, so just do a non-async polling transaction.
        const std::expected<void, ErrorCode> st = utils::convertHalStatus(
            HAL_SD_WriteBlocks(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, _timeout));
        return st;
    }

    if (taskInProgress != nullptr)
    {
        // There is a task currently in progress!
        return std::unexpected(ErrorCode::BUSY);
    }

    // Save current task before starting a SD transaction.
    taskInProgress = xTaskGetCurrentTaskHandle();

    std::expected<void, ErrorCode> exit = utils::convertHalStatus(
        HAL_SD_WriteBlocks_IT(&_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
    if (not exit.has_value())
    {
        // Mark this transaction as no longer in progress.
        taskInProgress = nullptr;
        return exit;
    }

    exit = waitForNotification(_timeout);
    return exit;
}

std::expected<void, ErrorCode>
    SdCard::writeOffset(const std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset) const
{
    if (pdata.empty())
        return {};

    CHECK_SD_PRESENT();
    if (not OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return std::unexpected(ErrorCode::ERROR);
    return write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
}

std::expected<void, ErrorCode> SdCard::erase(const uint32_t start_addr, const uint32_t end_addr) const
{
    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(&_hsd) != HAL_SD_CARD_TRANSFER)
        ;
    // TODO I think this needs to be a tx?
    return hw::utils::convertHalStatus(HAL_SD_Erase(&_hsd, start_addr, end_addr));
}

std::expected<void, ErrorCode> SdCard::writeDma(const std::span<uint8_t> pdata, const uint32_t block_addr) const
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
        // TODO set error
        LOG_ERROR("error: 0x%lX, %s", HAL_SD_GetError(&sd.getHsd()), sd.getErrorString());
    }

    void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        // TODO set error
        LOG_INFO("aborted");
    }
} // extern C
