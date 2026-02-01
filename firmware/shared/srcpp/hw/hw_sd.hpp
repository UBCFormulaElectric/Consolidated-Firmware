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
    SD_HandleTypeDef *const _hsd;          /* HAL SD handle that holds the state of the SD card */
    uint32_t                _timeout;      /* the timeout for the SD card operations */
    const Gpio             &_present_gpio; /* gpio for sd_cd */
    mutable volatile bool   dma_tx_completed = true;
    mutable volatile bool   dma_rx_completed = true;

    static bool OFFSET_SIZE_VALID(const uint32_t offset, const uint32_t size)
    {
        return (offset % HW_DEVICE_SECTOR_SIZE == 0) && (size % HW_DEVICE_SECTOR_SIZE == 0);
    }

#define CHECK_SD_PRESENT()                            \
    {                                                 \
        if (!sdPresent())                             \
        {                                             \
            return std::unexpected(ErrorCode::ERROR); \
        }                                             \
    }

  public:
    /* Constructor */
    consteval explicit SdCard(SD_HandleTypeDef *const hsd, const uint32_t timeout, const Gpio &present_gpio)
      : _hsd(hsd), _timeout(timeout), _present_gpio(present_gpio)
    {
    }

    /* Getters for private fields */
    SD_HandleTypeDef *getHsd() const { return _hsd; }

    uint32_t getTimeout() const { return _timeout; }

    const Gpio &getPresentGpio() const { return _present_gpio; }

    /* Setters for private fields */
    void setDmaTxCompleted(const bool value) const
    {
        dma_tx_completed = value;
        // TODO signaling to blocked here
    }
    void setDmaRxCompleted(const bool value) const
    {
        dma_rx_completed = value;
        // TODO signaling to blocked here
    }

    /**
     * @brief   Read from sd card.
     * @param   pdata the span where the read data is stored;
     *                should reserve [pdata, pdata + num_blocks * HW_DEVICE_SECTOR_SIZE] of memory space
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @return  the ExitCode of the opeation
     *
     */
    std::expected<void, ErrorCode> read(std::span<uint8_t> pdata, uint32_t block_addr) const;

    /**
     * @brief   Write to the sd card.
     * @param   pdata the span with data to write;
     *                should reserve [pdata, pdata+num_blocks * HW_DEVICE_SECTOR_SIZE] of memory space
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @return  the ExitCode of the opeation
     *
     */
    std::expected<void, ErrorCode> write(std::span<uint8_t> pdata, uint32_t block_addr) const;
    std::expected<void, ErrorCode> writeDma(std::span<uint8_t> pdata, uint32_t block_addr) const;

    /**
     * @brief   Read interface with offset and size argument, interface for littlefs
     * @param   pdata the span where the read data is stored;
     *                the data in the address space [pdata, pdata + size] will be copied to sd card
     * @param   block_addr the index of the block on sd card; must greater then 0
     * @param   offset offset within a block; must be divisible by HW_DEVICE_SECTOR_SIZE
     * @return  the ExitCode of the opeation
     *
     */
    std::expected<void, ErrorCode> readOffset(std::span<uint8_t> pdata, uint32_t block_addr, uint32_t offset) const;

    /**
     * @brief   Write interface with offset and size, interface for littlefs
     * @param   pdata the span with data to write;
     *                the data in the address space [pdata, pdata + size] will be copied to sd card
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @param   offset offset within a block; must be divisible by HW_DEVICE_SECTOR_SIZE
     * @return  the ExitCode of the opeation
     *
     */
    std::expected<void, ErrorCode> writeOffset(std::span<uint8_t> pdata, uint32_t block_addr, uint32_t offset) const;

    /**
     * @brief  Erase data from the sd card [start_addr, end_addr], inclusive
     * @param  start_addr  start of block index
     * @param  end_addr end of block index
     * @return the ExitCode of the opeation
     */
    std::expected<void, ErrorCode> erase(uint32_t start_addr, uint32_t end_addr) const;

    /**
     * @brief  Detect if the sd card is present.
     * @return True if the card is inserted, false otherwise
     */
    bool sdPresent() const;

    /**
     * @brief   Abort the current operation
     * @return  the SdCardStatus of the opeation
     */
    std::expected<void, ErrorCode> abort() const;
};

/**
 * @brief   Return the SdCard instance with the matching handle
 * @param   hsd  the SD handle
 * @return  the SdCard instance as a reference
 */
const SdCard &getSdFromHandle(SD_HandleTypeDef *hsd);

} // namespace hw
