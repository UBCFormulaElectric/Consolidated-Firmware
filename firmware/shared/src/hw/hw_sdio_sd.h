#pragma once

#include "hw_hal.h"
#include <stdbool.h>
#include <stdint.h>

// Check here for documentation about the type defined by HAL
// https://www.disca.upv.es/aperles/arm_cortex_m3/llibre/st/STM32F439xx_User_Manual/structsd__handletypedef.html

typedef struct
{
    SD_HandleTypeDef *const hsd;     // the HAL SD handle that will hold the state of the SD card
    uint32_t                timeout; // the timeout for the SD card operations
} SD_card;                           // struct that included all the state about SDIO and SD card

typedef struct
{
    SD_TypeDef *   sdio;        // sdio register base address
    SD_InitTypeDef sdio_config; // sdio config
    uint32_t       timeout;     // timeout config
} SD_card_init_config;

typedef enum
{
    SD_card_OK      = HAL_OK,
    SD_card_ERROR   = HAL_ERROR,
    SD_card_BUSY    = HAL_BUSY,
    SD_card_TIMEOUT = HAL_TIMEOUT
} SD_card_status; // wrapper of HAL status for better interface

/**
 * @brief Initialize and config the SDIO, initilize and figure out the card information
 * @param sdio the pointer to the SDIO peripheral registers
 * @param sd_config will configure the SDIO peripheral
 * @param card_info will hold the information about the SD card
 * @return SD_card* the pointer to the SD card structure or NULL when something goes wrong
 *
 */
SD_card hw_sd_init(SD_card_init_config config);

/**
 * @brief   Read from sd card. The data size will be num_blocks * BlockSize
 * @param   sd the state struct of sd card
 * @param   pdata the base addr where the read data store to;
 *                should reserve [pdata, pdata+num_blocks * BlockSize] of memory space
 * @param   block_addr the index of the block on sd card must greater then 0
 * @param   num_blocks number of block you want to read
 * @return  SD_card_status the status of the opeation
 *
 */
SD_card_status hw_sd_read(SD_card *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

/**
 * @brief   Write from the sd card
 * @param   sd the state struct of sd card
 * @param   pdata the base addr where data write to;
 *                the data in the address space [pdata, pdata + num_blocks * BlockSize] will be copy to sd card
 * @param   block_addr the index of the block on sd card must greater then 0
 * @param   num_blocks number of block you want to read
 * @return SD_card_status the status of the opeation
 *
 */
SD_card_status hw_sd_write(SD_card *sd, uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

/**
 * @brief Erase data from the sd card
 *
 * @param sd he state struct of sd card
 * @param start_addr  start of block index
 * @param end_addr start of block index
 * @return SD_card_status the status of the opeation
 */
SD_card_status hw_sd_erase(SD_card *sd, uint32_t start_addr, uint32_t end_addr);
