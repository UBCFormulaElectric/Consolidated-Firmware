
// interface read  write
// using HAL layer
#include "hw_sdio_sd.h"
#include <stdlib.h>
#include <string.h>

SdCardStatus hw_sd_read(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_ReadBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}

SdCardStatus hw_sd_readOffset(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size)
{
    uint32_t     block_size = sd->hsd->SdCard.BlockSize;
    SdCardStatus status     = SD_CARD_OK;

    if (size == 0)
    {
        return SD_CARD_OK;
    }

    if (offset == 0 && (block_size - size) == 0) // easy case
    {
        status = hw_sd_read(sd, pdata, block_addr, size / block_size);
        return (SdCardStatus)status;
    }

    uint8_t local_buffer[block_size];

    status = hw_sd_read(sd, local_buffer, block_addr, 1); // write the whole
    if (status != SD_CARD_OK)
        return status;

    memcpy(pdata, local_buffer + offset, size); // copy a section of data out

    return status;
}

SdCardStatus hw_sd_write(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_WriteBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}

SdCardStatus hw_sd_writeOffset(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size)
{
    uint32_t     block_size = sd->hsd->SdCard.BlockSize;
    SdCardStatus status     = SD_CARD_OK;
    if (size == 0)
    {
        return status;
    }

    if (offset == 0 && (block_size - size) == 0) // easy case
    {
        status = hw_sd_write(sd, pdata, block_addr, size / block_size);
        return status;
    }

    uint8_t local_buffer[block_size];

    status = hw_sd_read(sd, local_buffer, block_addr, 1); // read the whole block
    if (status != SD_CARD_OK)
        return status;

    memcpy(local_buffer + offset, pdata, size);            // write to local buffer from offet to offset + size
    status = hw_sd_write(sd, local_buffer, block_addr, 1); // write back

    return status;
}

SdCardStatus hw_sd_erase(SdCard *sd, uint32_t start_addr, uint32_t end_addr)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_Erase(sd->hsd, start_addr, end_addr);

    return (SdCardStatus)status;
}
