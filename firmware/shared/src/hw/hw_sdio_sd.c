// interface read  write
// using HAL layer
#include "hw_sdio_sd.h"
#include <stdlib.h>

SdCard *hw_sdio_sd_init(SdCardInitConfig config)
{
    SdCard *sd =
        malloc(sizeof(SdCard)); // maybe replaced by other memory allocation function or don't do malloc at all

    if (sd == NULL)
    {
        return NULL;
    }

    sd->hsd->Instance = config.sdio;
    sd->hsd->Init     = config.sdio_config;
    sd->timeout       = config.timeout;

    // will init and configure the SDIO peripheral
    // will use SDIO to send command to figure out the card information
    HAL_StatusTypeDef status = HAL_SD_Init(sd->hsd);
    if (status != HAL_OK)
    {
        return NULL;
    }

    return sd;
}

SdCardStatus hw_sd_read(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_ReadBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}
SdCardStatus hw_sd_write(SdCard *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_WriteBlocks(sd->hsd, pdata, block_addr, num_blocks, sd->timeout);

    return (SdCardStatus)status;
}
SdCardStatus hw_sd_erase(SdCard *sd, uint32_t start_addr, uint32_t end_addr)
{
    while (HAL_SD_GetCardState(sd->hsd) != HAL_SD_CARD_READY)
        ;

    HAL_StatusTypeDef status = HAL_SD_Erase(sd->hsd, start_addr, end_addr);

    return (SdCardStatus)status;
}
