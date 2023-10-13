#pragma once

#include "hw_hal.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    SD_HandleTypeDef *const sd;      // the HAL SD handle that will hold the state of the SD card
    uint32_t                timeout; // the timeout for the SD card operations
} SD_card;                           // struct that included all the state about SDIO and SD card

typedef struct
{
    SD_TypeDef *           sdio;        // HAL sdio register base address
    SD_InitTypeDef         sdio_config; // sdio config
    HAL_SD_CardInfoTypeDef card_info;   // SD card config
    uint32_t               timeout;     // timeout config
} SD_card_init_config;

// Check here fore documentation about SD_HandleTypeDef
// https://www.disca.upv.es/aperles/arm_cortex_m3/llibre/st/STM32F439xx_User_Manual/structsd__handletypedef.html

/**
 * @brief Initialize the SDIO peripheral
 * @param sdio the pointer to the SDIO peripheral registers
 * @param sd_config will configure the SDIO peripheral
 * @param card_info will hold the information about the SD card
 * @return SD_card* the pointer to the SD card structure
 *
 */
SD_card hw_sd_init(SD_card_init_config config);

/**
 * @brief   read from sd card from a block addr and store to a place
 * @param   states of sd card
 * @param
 * @param
 * @param
 *
 */
void hw_sd_read(SD_card *sd, uint8_t *data, uint32_t block_addr, uint32_t num_blocks);
void hw_sd_write(SD_card *sd, uint8_t *data, uint32_t block_addr, uint32_t num_blocks);
void hw_sd_erase(SD_card *sd, uint32_t start_addr, uint32_t end_addr);
