#include "hw_sd.hpp"

#include <cassert>
#include <cstdint>

namespace hw
{
ExitCode SdCard::read(std::span<uint8_t> pdata, const uint32_t block_addr)
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return ExitCode::EXIT_CODE_ERROR;

    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER)
        ;
    HAL_StatusTypeDef status =
        HAL_SD_ReadBlocks(hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, timeout);
    return hw_utils_convertHalStatus(status);
}

ExitCode SdCard::readOffset(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset)
{
    CHECK_SD_PRESENT();
    if (OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return SdCard::read(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
    return ExitCode::EXIT_CODE_ERROR;
}

ExitCode SdCard::write(std::span<uint8_t> pdata, const uint32_t block_addr)
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return ExitCode::EXIT_CODE_ERROR;

    while (HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER)
        ;
    HAL_StatusTypeDef status =
        HAL_SD_WriteBlocks(hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE, timeout);
    return hw_utils_convertHalStatus(status);
}

ExitCode SdCard::writeOffset(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset)
{
    if (pdata.size() == 0)
        return ExitCode::EXIT_CODE_OK;

    CHECK_SD_PRESENT();
    if (OFFSET_SIZE_VALID(offset, pdata.size())) // easy case
        return SdCard::write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE);
    return ExitCode::EXIT_CODE_ERROR;
}

ExitCode SdCard::erase(const uint32_t start_addr, const uint32_t end_addr)
{
    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return hw_utils_convertHalStatus(HAL_SD_Erase(hsd, start_addr, end_addr));
}

ExitCode SdCard::writeDma(std::span<uint8_t> pdata, const uint32_t block_addr)
{
    if (pdata.size() < HW_DEVICE_SECTOR_SIZE || pdata.size() % HW_DEVICE_SECTOR_SIZE != 0)
        return ExitCode::EXIT_CODE_ERROR;

    CHECK_SD_PRESENT();
    while (!dma_tx_completed)
        ;
    while (HAL_SD_GetCardState(hsd) != HAL_SD_CARD_TRANSFER)
        ;

    dma_tx_completed = false;
    return hw_utils_convertHalStatus(
        HAL_SD_WriteBlocks_DMA(hsd, pdata.data(), block_addr, pdata.size() / HW_DEVICE_SECTOR_SIZE));
}

// Based on the hardware design: if the sd card is inserted, the gpio will be shorted to ground. Otherwise it will be
// pulled up
bool SdCard::sdPresent(void)
{
    return !present_gpio->readPin();
}

ExitCode SdCard::abort(void)
{
    CHECK_SD_PRESENT();
    HAL_StatusTypeDef status = HAL_SD_Abort(hsd);
    return hw_utils_convertHalStatus(status);
}
} // namespace hw

/* HAL callbacks are not inside hw namespace */
extern "C"
{
    void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
    {
        hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaTxCompleted(true);
    }

    void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
    {
        hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaRxCompleted(true);
    }

    void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
    {
        hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaTxCompleted(true);
    }

    void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
    {
        hw::SdCard &sd = hw::getSdFromHandle(hsd);
        sd.setDmaTxCompleted(true);
    }
} // extern C
