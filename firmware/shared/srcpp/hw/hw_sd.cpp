#include "hw_sd.hpp"

#include <cassert>
#include <cstdint>

namespace hw
{
std::expected<void, ErrorCode> SdCard::read(std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(_hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return hw_utils_convertHalStatus(
        HAL_SD_ReadBlocks(_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, _timeout));
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

    while (HAL_SD_GetCardState(_hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return hw_utils_convertHalStatus(
        HAL_SD_WriteBlocks(_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, _timeout));
}

std::expected<void, ErrorCode>
    SdCard::writeOffset(const std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset) const
{
    if (pdata.size() == 0)
        return {};

    CHECK_SD_PRESENT();
    if (not OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return std::unexpected(ErrorCode::ERROR);
    return write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
}

std::expected<void, ErrorCode> SdCard::erase(const uint32_t start_addr, const uint32_t end_addr) const
{
    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(_hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return hw_utils_convertHalStatus(HAL_SD_Erase(_hsd, start_addr, end_addr));
}

std::expected<void, ErrorCode> SdCard::writeDma(const std::span<uint8_t> pdata, const uint32_t block_addr) const
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return std::unexpected(ErrorCode::ERROR);

    CHECK_SD_PRESENT();
    while (!dma_tx_completed)
        ;
    while (HAL_SD_GetCardState(_hsd) != HAL_SD_CARD_TRANSFER)
        ;

    dma_tx_completed = false;
    return hw_utils_convertHalStatus(
        HAL_SD_WriteBlocks_DMA(_hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
}

// Based on the hardware design: if the sd card is inserted, the gpio will be shorted to ground. Otherwise it will be
// pulled up
bool SdCard::sdPresent() const
{
    return !_present_gpio.readPin();
}

std::expected<void, ErrorCode> SdCard::abort() const
{
    CHECK_SD_PRESENT();
    return hw_utils_convertHalStatus(HAL_SD_Abort(_hsd));
}
} // namespace hw

/* HAL callbacks are not inside hw namespace */
extern "C"
{
    void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaTxCompleted(true);
    }

    void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaRxCompleted(true);
    }

    void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaTxCompleted(true);
    }

    void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
    {
        const hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaTxCompleted(true);
    }
} // extern C
