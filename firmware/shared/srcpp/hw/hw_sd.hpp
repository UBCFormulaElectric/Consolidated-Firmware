#pragma once

#include "hw_hal.hpp"
#include "hw_gpio.hpp"
#include "hw_utils.hpp"

#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_sd.h"
#include "stm32h5xx_hal_sd_ex.h"

#include <cstdint>

#define HW_DEVICE_SECTOR_SIZE 512

namespace hw
{
enum class SdCardStatus
{
    SD_CARD_OK      = HAL_OK,
    SD_CARD_ERROR   = HAL_ERROR,
    SD_CARD_BUSY    = HAL_BUSY,
    SD_CARD_TIMEOUT = HAL_TIMEOUT
};

class SdCard
{
    public:
        SD_HandleTypeDef *hsd;          // the HAL SD handle that will hold the state of the SD card
        uint32_t          timeout;      // the timeout for the SD card operations
        const Gpio       *present_gpio; // gpio for sd_cd

        /**
         * @brief   Read from sd card. The data size will be num_blocks * BlockSize
         * @param   pdata the base addr where the read data store to;
         *                should reserve [pdata, pdata + num_blocks * BlockSize] of memory space
         * @param   block_addr the index of the block on sd card; must be greater then 0
         * @param   num_blocks number of block you want to read
         * @return  the SdCardStatus of the opeation
         *
         */
        SdCardStatus hw_sd_read(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

        /**
         * @brief   Write to the sd card. The data size will be num_blocks * BlockSize
         * @param   sd the state struct of sd card
         * @param   pdata the base addr where to write data;
         *                should reserve [pdata, pdata+num_blocks * BlockSize] of memory space
         * @param   block_addr the index of the block on sd card; must be greater then 0
         * @param   num_blocks number of block you want to write
         * @return  the SdCardStatus status of the opeation
         *
         */
        SdCardStatus hw_sd_write(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);
        SdCardStatus hw_sd_writeDma(uint8_t *pdata, uint32_t block_addr, uint32_t num_blocks);

        /**
         * @brief   Read interface with offset and size argument, interface for littlefs
         * @param   pdata the base addr where data write to;
         *                the data in the address space [pdata, pdata + size] will be copied to sd card
         * @param   block_addr the index of the block on sd card; must greater then 0
         * @param   offset offset within a block; must be divisible by BLOCK_SIZE
         * @param   size   must be divisible by BLOCK_SIZE
         * @return  the SdCardStatus of the opeation
         *
         */
        SdCardStatus hw_sd_readOffset(uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size);

        /**
         * @brief   Write interface with offset and size, interface for littlefs
         * @param   pdata the base addr where data write to;
         *                the data in the address space [pdata, pdata + size] will be copied to sd card
         * @param   block_addr the index of the block on sd card; must be greater then 0
         * @param   offset offset within a block; must be divisible by BLOCK_SIZE
         * @param   size   bytes of the data write to size; must be divisible by BLOCK_SIZE
         * @return  the SdCardStatus of the opeation
         *
         */
        SdCardStatus hw_sd_writeOffset(uint8_t *pdata, uint32_t block_addr, uint32_t offset, uint32_t size);

        /**
         * @brief  Erase data from the sd card [start_addr, end_addr], inclusive
         * @param  start_addr  start of block index
         * @param  end_addr end of block index
         * @return the SdCardStatus of the opeation
         */
        SdCardStatus hw_sd_erase(uint32_t start_addr, uint32_t end_addr);

        /**
         * @brief  Detect if the sd card is present.
         * @return True if the card is inserted, false otherwise
         */
        bool hw_sd_present(void);

        /**
         * @brief   Abort the current operation
         * @return  the SdCardStatus of the opeation
         */
        SdCardStatus hw_sd_abort(void);
};
} // namespace hw
