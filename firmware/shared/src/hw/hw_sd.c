
// interface read  write
// using HAL layer
#include "hw_sd.h"
#include "hw_gpio.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern bool sd_init;

static SdCard *sd;

void hw_sd_init(SdCard *sd_config)
{
    sd = sd_config;
    if (sd_init)
    {
        hw_sd_markInitComplete();
    }
}

bool hw_sd_checkSdPresent(void)
{
    return !hw_gpio_readPin(&sd->sd_present);
}

void hw_sd_markInitComplete(void)
{
    sd->sd_init_complete = true;
}

bool hw_sd_sdReady(void)
{
    return sd->sd_init_complete && hw_sd_checkSdPresent();
}

SdCardStatus hw_sd_read(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_TRANSFER)
        ;

    HAL_StatusTypeDef status = HAL_SD_ReadBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}

SdCardStatus hw_sd_readOffset(uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size)
{
    SdCardStatus status = SD_CARD_OK;

    if (size == 0)
    {
        return SD_CARD_OK;
    }

    if (((offset % HW_DEVICE_SECTOR_SIZE) == 0) && (size % HW_DEVICE_SECTOR_SIZE == 0)) // easy case
    {
        status = hw_sd_read(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE, size / HW_DEVICE_SECTOR_SIZE);
        return (SdCardStatus)status;
    }

    return SD_CARD_ERROR;
}


SdCardStatus hw_sd_write(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_TRANSFER)
        ;

    HAL_StatusTypeDef status = HAL_SD_WriteBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}

SdCardStatus hw_sd_writeOffset(uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size)
{
    SdCardStatus status = SD_CARD_OK;
    if (size == 0)
    {
        return status;
    }

    if (((offset % HW_DEVICE_SECTOR_SIZE) == 0) && (size % HW_DEVICE_SECTOR_SIZE == 0)) // easy case
    {
        status = hw_sd_write(pdata, block_addr + offset / HW_DEVICE_SECTOR_SIZE, size / HW_DEVICE_SECTOR_SIZE);
        return status;
    }

    return SD_CARD_ERROR;
}

SdCardStatus hw_sd_erase(uint32_t start_addr, uint32_t end_addr)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_TRANSFER)
        ;

    HAL_StatusTypeDef status = HAL_SD_Erase(sd->hsd, start_addr, end_addr);

    return (SdCardStatus)status;
}
