#include "hw_sd.hpp"

#include <cassert>
#include <cstdint>

namespace hw
{
/**
 * @attention This must be constructed in some translation unit in each ECU which uses includes file
 * @note create this object in hw_sds.c for your ECU
 */
extern SdCard sd1;

static volatile bool dma_tx_completed = true;

SdCardStatus SdCard::read(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t num_blocks)
{
    if (pdata.data() == nullptr || pdata.empty() || num_blocks == 0)
        return hw::SdCardStatus::SD_CARD_ERROR;

    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;
    HAL_StatusTypeDef status = HAL_SD_ReadBlocks(sd1.hsd, pdata.data(), block_addr, num_blocks, sd1.timeout);
    return getSdStatus(status);
}

SdCardStatus SdCard::readOffset(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset, const uint32_t size)
{
    if (size == 0)
        return hw::SdCardStatus::SD_CARD_OK;

    CHECK_SD_PRESENT();
    if (OFFSET_SIZE_VALID(offset, size)) // easy case
        return SdCard::read(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE, size / HW_DEVICE_SECTOR_SIZE);
    return hw::SdCardStatus::SD_CARD_ERROR;
}

SdCardStatus SdCard::write(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t num_blocks)
{
    if (pdata.data() == nullptr || pdata.empty() || num_blocks == 0)
        return hw::SdCardStatus::SD_CARD_ERROR;

    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;
    HAL_StatusTypeDef status = HAL_SD_WriteBlocks(sd1.hsd, pdata.data(), block_addr, num_blocks, sd1.timeout);
    return getSdStatus(status);
}

SdCardStatus SdCard::writeOffset(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t offset, const uint32_t size)
{
    if (size == 0)
        return hw::SdCardStatus::SD_CARD_OK;

    CHECK_SD_PRESENT();
    if (OFFSET_SIZE_VALID(offset, size)) // easy case
        return SdCard::write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE, size / HW_DEVICE_SECTOR_SIZE);
    return hw::SdCardStatus::SD_CARD_ERROR;
}

SdCardStatus SdCard::erase(const uint32_t start_addr, const uint32_t end_addr)
{
    CHECK_SD_PRESENT();
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return getSdStatus(HAL_SD_Erase(sd1.hsd, start_addr, end_addr));
}

SdCardStatus SdCard::writeDma(std::span<uint8_t> pdata, const uint32_t block_addr, const uint32_t num_blocks)
{
    if (pdata.data() == nullptr || pdata.empty() || num_blocks == 0)
        return hw::SdCardStatus::SD_CARD_ERROR;

    CHECK_SD_PRESENT();
    while (!dma_tx_completed)
        ;
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;

    dma_tx_completed = false;
    return getSdStatus(HAL_SD_WriteBlocks_DMA(sd1.hsd, pdata.data(), block_addr, num_blocks));
}

// Based on the hardware design: if the sd card is inserted, the gpio will be shorted to ground. Otherwise it will be pulled up
bool SdCard::sd_present(void)
{
    return !hw::sd1.present_gpio->readPin();
}

SdCardStatus SdCard::abort(void)
{
    CHECK_SD_PRESENT();
    HAL_StatusTypeDef status = HAL_SD_Abort(sd1.hsd);
    return getSdStatus(status);
}

SdCardStatus SdCard::getSdStatus(HAL_StatusTypeDef hal_status)
{
    SdCardStatus sd_status = SdCardStatus::SD_CARD_OK;
    switch (hal_status)
    {
        case HAL_OK:
            sd_status = SdCardStatus::SD_CARD_OK;
            break;
        case HAL_ERROR:
            sd_status = SdCardStatus::SD_CARD_ERROR;
            break;
        case HAL_BUSY:
            sd_status = SdCardStatus::SD_CARD_BUSY;
            break;
        case HAL_TIMEOUT:
            sd_status = SdCardStatus::SD_CARD_TIMEOUT;
            break;
        default:
            sd_status = SdCardStatus::SD_CARD_ERROR;
            break;
    }
    return sd_status;
}

} // namespace hw

/* HAL callbacks are not inside hw namespace */
extern "C"
{
    void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
    {
        assert(hsd == hw::sd1.getHsd());
        hw::dma_tx_completed = true;
    }

    void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
    {
        assert(hsd == hw::sd1.getHsd());
        hw::dma_tx_completed = true;
    }

    void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
    {
        assert(hsd == hw::sd1.getHsd());
        hw::dma_tx_completed = true;
    }

    void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
    {
        assert(hsd == hw::sd1.getHsd());
        hw::dma_tx_completed = true;
    }

} // extern C
