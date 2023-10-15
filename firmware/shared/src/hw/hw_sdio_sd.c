// interface read  write
// using HAL layer
#include "hw_sdio_sd.h"
#include <stdlib.h>

SD_card *hw_sdio_sd_init(SD_card_init_config config)
{
    SD_card *sd =
        malloc(sizeof(SD_card)); // maybe replaced by other memory allocation function or don't do malloc at all

    if (sd == NULL)
    {
        return NULL;
    }

    sd->sd->Instance = config.sdio;
    sd->sd->Init     = config.sdio_config;
    sd->sd->SdCard   = config.card_info;
    sd->timeout      = config.timeout;

    // will init and configure the SDIO peripheral
    // will init the SD card
    HAL_StatusTypeDef status = HAL_SD_Init(sd->sd);
    if (status != HAL_OK)
    {
        return NULL;
    }

    return sd;
}

SD_card_status hw_sd_read(SD_card *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    HAL_StatusTypeDef status = HAL_SD_ReadBlocks(sd->sd, pdata, block_addr, num_blocks, sd->timeout);

    return (SD_card_status)status;
}
SD_card_status hw_sd_write(SD_card *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks)
{
    HAL_StatusTypeDef status = HAL_SD_WriteBlocks(sd->sd, pdata, block_addr, num_blocks, sd->timeout);

    return (SD_card_status)status;
}
SD_card_status hw_sd_erase(SD_card *sd, uint32_t start_addr, uint32_t end_addr)
{
    HAL_StatusTypeDef status = HAL_SD_Erase(sd->sd, start_addr, end_addr);
    return (SD_card_status)status;
}
