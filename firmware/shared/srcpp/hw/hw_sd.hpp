#pragma once

#include "hw_hal.hpp"
#include "hw_gpio.hpp"
#include "hw_utils.hpp"

#ifdef STM32H562xx
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_sd.h"
#include "stm32h5xx_hal_sd_ex.h"
#endif

#include <cstdint>
#include <span>

constexpr int HW_DEVICE_SECTOR_SIZE = 512;

namespace hw
{
class SdCard
{
  private:
    SD_HandleTypeDef *hsd;          /* HAL SD handle that holds the state of the SD card */
    uint32_t          timeout;      /* the timeout for the SD card operations */
    const Gpio       *present_gpio; /* gpio for sd_cd */
    volatile bool     dma_tx_completed = true;

    static inline bool OFFSET_SIZE_VALID(uint32_t offset, uint32_t size)
    {
        return (offset % HW_DEVICE_SECTOR_SIZE == 0) && (size % HW_DEVICE_SECTOR_SIZE == 0);
    }

    inline ExitCode CHECK_SD_PRESENT()
    {
        if (!sdPresent())
            return ExitCode::EXIT_CODE_ERROR;
    }

  public:
    /* Constructor */
    constexpr SdCard(SD_HandleTypeDef *hsd, uint32_t timeout, const Gpio *present_gpio)
      : hsd(hsd), timeout(timeout), present_gpio(present_gpio)
    {
    }

    /* Getters for private fields */
    SD_HandleTypeDef *getHsd() const { return hsd; }

    uint32_t getTimeout() const { return timeout; }

    const Gpio *getPresentGpio() const { return present_gpio; }

    /* Setters for private fields */
    void setDmaTxCompleted(bool value) { dma_tx_completed = value; }

    /**
     * @brief   Read from sd card.
     * @param   pdata the span where the read data is stored;
     *                should reserve [pdata, pdata + num_blocks * HW_DEVICE_SECTOR_SIZE] of memory space
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @return  the ExitCode of the opeation
     *
     */
    ExitCode read(std::span<uint8_t> pdata, uint32_t block_addr);

    /**
     * @brief   Write to the sd card.
     * @param   pdata the span with data to write;
     *                should reserve [pdata, pdata+num_blocks * HW_DEVICE_SECTOR_SIZE] of memory space
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @return  the ExitCode of the opeation
     *
     */
    ExitCode write(std::span<uint8_t> pdata, uint32_t block_addr);
    ExitCode writeDma(std::span<uint8_t> pdata, uint32_t block_addr);

    /**
     * @brief   Read interface with offset and size argument, interface for littlefs
     * @param   pdata the span where the read data is stored;
     *                the data in the address space [pdata, pdata + size] will be copied to sd card
     * @param   block_addr the index of the block on sd card; must greater then 0
     * @param   offset offset within a block; must be divisible by HW_DEVICE_SECTOR_SIZE
     * @return  the ExitCode of the opeation
     *
     */
    ExitCode readOffset(std::span<uint8_t> pdata, uint32_t block_addr, uint32_t offset);

    /**
     * @brief   Write interface with offset and size, interface for littlefs
     * @param   pdata the span with data to write;
     *                the data in the address space [pdata, pdata + size] will be copied to sd card
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @param   offset offset within a block; must be divisible by HW_DEVICE_SECTOR_SIZE
     * @return  the ExitCode of the opeation
     *
     */
    ExitCode writeOffset(std::span<uint8_t> pdata, uint32_t block_addr, uint32_t offset);

    /**
     * @brief  Erase data from the sd card [start_addr, end_addr], inclusive
     * @param  start_addr  start of block index
     * @param  end_addr end of block index
     * @return the ExitCode of the opeation
     */
    ExitCode erase(uint32_t start_addr, uint32_t end_addr);

    /**
     * @brief  Detect if the sd card is present.
     * @return True if the card is inserted, false otherwise
     */
    bool sdPresent(void);

    /**
     * @brief   Abort the current operation
     * @return  the SdCardStatus of the opeation
     */
    ExitCode abort(void);
};

/**
 * @brief   Return the SdCard instance with the matching handle
 * @param   hsd  the SD handle
 * @return  the SdCard instance
 */
static SdCard *getSdFromHandle(SD_HandleTypeDef *hsd);

} // namespace hw
