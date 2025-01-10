// interface read  write
// using HAL layer
#include "hw_sd.h"
#include "hw_utils.h"

#include <assert.h>
#include <stdbool.h>

static volatile bool dma_tx_completed = true;
#define OFFSET_SIZE_VALID(offset, size) (offset % HW_DEVICE_SECTOR_SIZE == 0 && size % HW_DEVICE_SECTOR_SIZE == 0)

/**
 * @attention This must be constructed in some translation unit in each ECU which uses includes file
 * @note create this object in hw_sds.c for your ECU
 */
extern SdCard sd1;

SdCardStatus hw_sd_read(uint8_t *pdata, const uint32_t block_addr, const uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return (SdCardStatus)HAL_SD_ReadBlocks(sd1.hsd, pdata, block_addr, num_blocks, sd1.timeout);
}

SdCardStatus hw_sd_readOffset(uint8_t *pdata, const uint32_t block_addr, const uint32_t offset, const uint32_t size)
{
    if (size == 0)
        return SD_CARD_OK;
    if (OFFSET_SIZE_VALID(offset, size)) // easy case
        return hw_sd_read(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE, size / HW_DEVICE_SECTOR_SIZE);
    return SD_CARD_ERROR;
}

SdCardStatus hw_sd_write(uint8_t *pdata, const uint32_t block_addr, const uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return (SdCardStatus)HAL_SD_WriteBlocks(sd1.hsd, pdata, block_addr, num_blocks, sd1.timeout);
}

SdCardStatus hw_sd_writeOffset(uint8_t *pdata, const uint32_t block_addr, const uint32_t offset, const uint32_t size)
{
    if (size == 0)
        return SD_CARD_OK;
    if (OFFSET_SIZE_VALID(offset, size)) // easy case
        return hw_sd_write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE, size / HW_DEVICE_SECTOR_SIZE);
    return SD_CARD_ERROR;
}

SdCardStatus hw_sd_erase(const uint32_t start_addr, const uint32_t end_addr)
{
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;
    return (SdCardStatus)HAL_SD_Erase(sd1.hsd, start_addr, end_addr);
}

SdCardStatus hw_sd_writeDma(uint8_t *pdata, const uint32_t block_addr, const uint32_t num_blocks)
{
    while (!dma_tx_completed)
        ;
    while (HAL_SD_GetCardState(sd1.hsd) != HAL_SD_CARD_TRANSFER)
        ;

    dma_tx_completed = false;
    return (SdCardStatus)HAL_SD_WriteBlocks_DMA(sd1.hsd, pdata, block_addr, num_blocks);
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    assert(hsd == sd1.hsd);
    dma_tx_completed = true;
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    assert(hsd == sd1.hsd);
    dma_tx_completed = true;
}

void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
    assert(hsd == sd1.hsd);
    dma_tx_completed = true;
}
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    assert(hsd == sd1.hsd);
    dma_tx_completed = true;
}

bool hw_sd_present()
{
    return hw_gpio_readPin(sd1.present_gpio);
}
