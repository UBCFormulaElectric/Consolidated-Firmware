#pragma once

#include "hw_hal.h"

#include "hw_gpio.h"
#include <stdbool.h>
#include <stdint.h>

// Check here for documentation about the type defined by HAL
// https://www.disca.upv.es/aperles/arm_cortex_m3/llibre/st/STM32F439xx_User_Manual/structsd__handletypedef.html

#define HW_DEVICE_SECTOR_SIZE 512

typedef struct
{
    SD_HandleTypeDef *hsd;        // the HAL SD handle that will hold the state of the SD card
    uint32_t          timeout;    // the timeout for the SD card operations
    const Gpio        sd_present; // GPIO to detect if SD card installed
    bool              sd_init_complete;
} SdCard; // struct that included all the state about SDIO and SD card

typedef enum
{
    SD_CARD_OK      = HAL_OK,
    SD_CARD_ERROR   = HAL_ERROR,
    SD_CARD_BUSY    = HAL_BUSY,
    SD_CARD_TIMEOUT = HAL_TIMEOUT
} SdCardStatus; // wrapper of HAL status for better interface

/**
 * @brief Initialize Static SD Card structure
 * @param sd_config SD Card Configuration
 */
void hw_sd_init(SdCard *sd_config);

/**
 * @brief Check for presence of SD card
 * @return true if SD card present
 */
bool hw_sd_checkSdPresent(void);

/**
 * @brief Mark SD Initialization Complete
 */
void hw_sd_markInitComplete(void);

/**
 * @brief Check if SD is ready for operation
 * @return true if initialization complete and SD present
 */
bool hw_sd_sdReady(void);

/* Assume sdio and sd card is initilized, SdCard have desired member*/

void hw_sd_init(const SdCard *sd);

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
SdCardStatus hw_sd_read(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

/**
 * @brief   write to sd card. The data size will be num_blocks * BlockSize
 * @param   sd the state struct of sd card
 * @param   pdata the base addr where the write data from
 *                should reserve [pdata, pdata+num_blocks * BlockSize] of memory space
 * @param   block_addr the index of the block on sd card must greater then 0
 * @param   num_blocks number of block you want to read
 * @return  SD_card_status the status of the opeation
 *
 */
SdCardStatus hw_sd_write(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

SdCardStatus hw_sd_writeDma(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

/**
 * @brief   Read interface with offset and size arguement, interface for littlefs
 * @param   sd the state struct of sd card
 * @param   pdata the base addr where data write to;
 *                the data in the address space [pdata, pdata + size] will be copy to sd card
 * @param   block_addr the index of the block on sd card must greater then 0
 * @param   offset offset within a block devisible by BLOCK_SIZE
 * @param   size   devisible by BLOCK_SIZE
 * @return SD_card_status the status of the opeation
 *
 */
SdCardStatus hw_sd_readOffset(uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size);

/**
 * @brief   write interface with offset and size, interface for littlefs
 *
 * @param   sd the state struct of sd card
 * @param   pdata the base addr where data write to;
 *                the data in the address space [pdata, pdata + size] will be copy to sd card
 * @param   block_addr the index of the block on sd card must greater then 0
 * @param   offset offset within a block devisible by BLOCK_SIZE
 * @param   size   bytes of the data write to size devisible by BLOCK_SIZE
 * @return SD_card_status the status of the opeation
 *
 */
SdCardStatus hw_sd_writeOffset(uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size);

/**
 * @brief Erase data from the sd card [start_addr, end_addr] inclusive
 *
 * @param sd he state struct of sd card
 * @param start_addr  start of block index
 * @param end_addr start of block index
 * @return SD_card_status the status of the opeation
 */
SdCardStatus hw_sd_erase(uint32_t start_addr, uint32_t end_addr);
