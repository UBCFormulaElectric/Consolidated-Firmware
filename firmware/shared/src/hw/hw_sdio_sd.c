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

SdCardStatus hw_sd_read_offset(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size)
{
    uint32_t     block_size = sd->hsd->SdCard.BlockSize;
    SdCardStatus status     = SD_CARD_OK;

    if (size == 0)
    {
        return SD_CARD_OK;
    }

    if (offset == 0 && (size % block_size) == 0) // easy case
    {
        status = hw_sd_read(sd, pdata, block_addr, size / block_size);
        return (SdCardStatus)status;
    }

    // not easy case, data is in between blockes
    uint32_t end          = offset + size;
    uint32_t total_size   = end + (abs(end - block_size)) % block_size; // around up the blockes
    uint8_t *local_buffer = malloc(total_size);                         // temp buffer
    if (local_buffer == NULL)
    {
        status = SD_CARD_ERROR;
        goto free_done;
    }

    status = (SdCardStatus)hw_sd_read(sd, local_buffer, block_addr, total_size / block_size);
    if (status != SD_CARD_OK)
    {
        goto free_done;
    }

    memcpy(pdata, local_buffer + offset, size);
free_done:
    free(local_buffer);
    return status;
}

SdCardStatus hw_sd_write(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_WriteBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}

SdCardStatus hw_sd_write_offset(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size)
{
    uint32_t     block_size = sd->hsd->SdCard.BlockSize;
    SdCardStatus status     = SD_CARD_OK;
    if (size == 0)
    {
        return status;
    }

    if (offset == 0 && (size % block_size) == 0) // easy case
    {
        status = hw_sd_write(sd, pdata, block_addr, size / block_size);
        return status;
    }

    // not easy case, the data is in between blocks
    uint32_t end = offset + size;

    // read first block and last block, put it together with write data, write to the sd card
    uint32_t total_size   = end + (abs(end - block_size)) % block_size;
    uint8_t *local_buffer = malloc(total_size);
    if (local_buffer == NULL)
        return SD_CARD_ERROR;

    if (total_size == block_size) // only one block
    {
        status = hw_sd_read(sd, local_buffer, block_addr, 1);
        memcpy(local_buffer + offset, pdata, size);
        status = hw_sd_write(sd, local_buffer, block_addr, 1);
        goto free_done;
    }

    uint32_t last_block_addr = block_addr + total_size / block_size - 1; // last

    // read first block and last block
    status = hw_sd_read(sd, local_buffer, block_addr, 1); // first block
    if (status != SD_CARD_OK)
        goto free_done;
    status =
        hw_sd_read(sd, local_buffer + (last_block_addr - block_addr) * block_size, last_block_addr, 1); // last block
    if (status != SD_CARD_OK)
        goto free_done;
    // copy the middle block
    memcpy(local_buffer + offset, pdata, size);
    status = hw_sd_write(sd, local_buffer, block_addr, total_size / block_size);

free_done:
    free(local_buffer);
    return status;
}

SdCardStatus hw_sd_erase(SdCard *sd, uint32_t start_addr, uint32_t end_addr)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_Erase(sd->hsd, start_addr, end_addr);

    return (SdCardStatus)status;
}
