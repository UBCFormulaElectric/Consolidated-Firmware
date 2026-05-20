#pragma once
#include "hw_gpio.hpp"
#include "hw_utils.hpp"

#include "FreeRTOS.h"
#include "task.h"

#ifdef STM32H562xx
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_sd.h"
// #include "stm32h5xx_hal_sd_ex.h"
#elifdef STM32H733xx
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_sd.h"
// #include "stm32h7xx_hal_sd_ex.h"
#endif

#include <cstdint>
#include <span>

inline constexpr int HW_DEVICE_SECTOR_SIZE = 512;

namespace hw
{
class SdCard
{
  private:
    SD_HandleTypeDef &_hsd;          /* HAL SD handle that holds the state of the SD card */
    uint32_t          _timeout;      /* the timeout for the SD card operations */
    const gpio       &_present_gpio; /* gpio for sd_cd */
    // mutable volatile bool   dma_tx_completed = true;
    // mutable volatile bool   dma_rx_completed = true;

    mutable TaskHandle_t taskInProgress = nullptr;

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

    result<void> waitForNotification(uint32_t timeoutMs) const;

  public:
    void onTransactionCompleteFromISR() const;

    /* Constructor */
    consteval explicit SdCard(SD_HandleTypeDef &hsd, const uint32_t timeout, const gpio &present_gpio)
      : _hsd(hsd), _timeout(timeout), _present_gpio(present_gpio)
    {
    }

    /* Getters for private fields */
    SD_HandleTypeDef &getHsd() const { return _hsd; }

    uint32_t getTimeout() const { return _timeout; }

    // const gpio &getPresentGpio() const { return _present_gpio; }

    [[nodiscard]] const char *getCardStateString() const
    {
        switch (HAL_SD_GetCardState(&_hsd))
        {
            case HAL_SD_CARD_READY:
                return "Card state is ready";
            case HAL_SD_CARD_IDENTIFICATION:
                return "Card is in identification state";
            case HAL_SD_CARD_STANDBY:
                return "Card is in standby state";
            case HAL_SD_CARD_TRANSFER:
                return "Card is in transfer state";
            case HAL_SD_CARD_SENDING:
                return "Card is sending an operation";
            case HAL_SD_CARD_RECEIVING:
                return "Card is receiving operation information";
            case HAL_SD_CARD_PROGRAMMING:
                return "Card is in programming state";
            case HAL_SD_CARD_DISCONNECTED:
                return "Card is disconnected";
            case HAL_SD_CARD_ERROR:
                return "Card response Error";
            default:
                return "Unknown card state";
        }
    }
    [[nodiscard]] const char *getErrorString() const
    {
        switch (HAL_SD_GetError(&_hsd))
        {
            case SDMMC_ERROR_NONE:
                return "No error";
            case SDMMC_ERROR_CMD_CRC_FAIL:
                return "Command response received (but CRC check failed)";
            case SDMMC_ERROR_DATA_CRC_FAIL:
                return "Data block sent/received (CRC check failed)";
            case SDMMC_ERROR_CMD_RSP_TIMEOUT:
                return "Command response timeout";
            case SDMMC_ERROR_DATA_TIMEOUT:
                return "Data timeout";
            case SDMMC_ERROR_TX_UNDERRUN:
                return "Transmit FIFO underrun";
            case SDMMC_ERROR_RX_OVERRUN:
                return "Receive FIFO overrun";
            case SDMMC_ERROR_ADDR_MISALIGNED:
                return "Misaligned address";
            case SDMMC_ERROR_BLOCK_LEN_ERR:
                return "Transferred block length is not allowed for the card";
            case SDMMC_ERROR_ERASE_SEQ_ERR:
                return "An error in the sequence of erase command occurs";
            case SDMMC_ERROR_BAD_ERASE_PARAM:
                return "An invalid selection for erase groups";
            case SDMMC_ERROR_WRITE_PROT_VIOLATION:
                return "Attempt to program a write protect block";
            case SDMMC_ERROR_LOCK_UNLOCK_FAILED:
                return "Sequence or password error in unlock command";
            case SDMMC_ERROR_COM_CRC_FAILED:
                return "CRC check of the previous command failed";
            case SDMMC_ERROR_ILLEGAL_CMD:
                return "Command is not legal for the card state";
            case SDMMC_ERROR_CARD_ECC_FAILED:
                return "Card internal ECC was applied but failed to correct the data";
            case SDMMC_ERROR_CC_ERR:
                return "Internal card controller error";
            case SDMMC_ERROR_GENERAL_UNKNOWN_ERR:
                return "General or unknown error";
            case SDMMC_ERROR_STREAM_READ_UNDERRUN:
                return "The card could not sustain data reading in stream mode";
            case SDMMC_ERROR_STREAM_WRITE_OVERRUN:
                return "The card could not sustain data programming in stream mode";
            case SDMMC_ERROR_CID_CSD_OVERWRITE:
                return "CID/CSD overwrite error";
            case SDMMC_ERROR_WP_ERASE_SKIP:
                return "Only partial address space was erased";
            case SDMMC_ERROR_CARD_ECC_DISABLED:
                return "Command has been executed without using internal ECC";
            case SDMMC_ERROR_ERASE_RESET:
                return "Erase sequence was cleared before executing";
            case SDMMC_ERROR_AKE_SEQ_ERR:
                return "Error in sequence of authentication";
            case SDMMC_ERROR_INVALID_VOLTRANGE:
                return "Error in case of invalid voltage range";
            case SDMMC_ERROR_ADDR_OUT_OF_RANGE:
                return "Error when addressed block is out of range";
            case SDMMC_ERROR_REQUEST_NOT_APPLICABLE:
                return "Error when command request is not applicable";
            case SDMMC_ERROR_INVALID_PARAMETER:
                return "The used parameter is not valid";
            case SDMMC_ERROR_UNSUPPORTED_FEATURE:
                return "Error when feature is not supported";
            case SDMMC_ERROR_BUSY:
                return "Error when transfer process is busy";
            case SDMMC_ERROR_DMA:
                return "Error while DMA transfer";
            case SDMMC_ERROR_TIMEOUT:
                return "Timeout error";
            default:
                return "Unknown error";
        }
    }

    std::expected<void, ErrorCode> upgrade_buswidth() const
    {
        const auto res = utils::convertHalStatus(HAL_SD_ConfigWideBusOperation(&_hsd, SDMMC_BUS_WIDE_4B));
        return res;
    }

    std::expected<void, ErrorCode> update_speed() const
    {
        RETURN_IF_ERR_SILENT(utils::convertHalStatus(HAL_SD_ConfigSpeedBusOperation(&_hsd, SDMMC_SPEED_MODE_HIGH)));
        __ISB();
        __DSB();
        constexpr uint32_t freq = 5'000'000;
        constexpr uint32_t x    = 250'000'000 / (2 * freq);
        MODIFY_REG(_hsd.Instance->CLKCR, SDMMC_CLKCR_CLKDIV, (x << SDMMC_CLKCR_CLKDIV_Pos));
        return {};
    }

    /**
     * @brief   Read from sd card.
     * @param   pdata the span where the read data is stored;
     *                should reserve [pdata, pdata + num_blocks * HW_DEVICE_SECTOR_SIZE] of memory space
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @return  the ExitCode of the opeation
     *
     */
    result<void> read(std::span<uint8_t> pdata, uint32_t block_addr) const;

    /**
     * @brief   Write to the sd card.
     * @param   pdata the span with data to write;
     *                should reserve [pdata, pdata+num_blocks * HW_DEVICE_SECTOR_SIZE] of memory space
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @return  the ExitCode of the opeation
     *
     */
    result<void> write(std::span<uint8_t> pdata, uint32_t block_addr) const;
    result<void> writeDma(std::span<uint8_t> pdata, uint32_t block_addr) const;

    /**
     * @brief   Read interface with offset and size argument, interface for littlefs
     * @param   pdata the span where the read data is stored;
     *                the data in the address space [pdata, pdata + size] will be copied to sd card
     * @param   block_addr the index of the block on sd card; must greater then 0
     * @param   offset offset within a block; must be divisible by HW_DEVICE_SECTOR_SIZE
     * @return  the ExitCode of the opeation
     *
     */
    result<void> readOffset(std::span<uint8_t> pdata, uint32_t block_addr, uint32_t offset) const;

    /**
     * @brief   Write interface with offset and size, interface for littlefs
     * @param   pdata the span with data to write;
     *                the data in the address space [pdata, pdata + size] will be copied to sd card
     * @param   block_addr the index of the block on sd card; must be greater then 0
     * @param   offset offset within a block; must be divisible by HW_DEVICE_SECTOR_SIZE
     * @return  the ExitCode of the opeation
     *
     */
    result<void> writeOffset(std::span<uint8_t> pdata, uint32_t block_addr, uint32_t offset) const;

    /**
     * @brief  Erase data from the sd card [start_addr, end_addr], inclusive
     * @param  start_addr  start of block index
     * @param  end_addr end of block index
     * @return the ExitCode of the opeation
     */
    result<void> erase(uint32_t start_addr, uint32_t end_addr) const;

    /**
     * @brief  Detect if the sd card is present.
     * @return True if the card is inserted, false otherwise
     * @note Based on the hardware design: if the sd card is inserted, the gpio will be shorted to ground. Otherwise it
     * will be pulled up
     */
    bool sdPresent() const { return _present_gpio.readPin(); }

    /**
     * @brief   Abort the current operation
     * @return  the SdCardStatus of the opeation
     */
    result<void> abort() const
    {
        CHECK_SD_PRESENT();
        return utils::convertHalStatus(HAL_SD_Abort(&_hsd));
    }
};

/**
 * @brief   Return the SdCard instance with the matching handle
 * @param   hsd  the SD handle
 * @return  the SdCard instance as a reference
 */
const SdCard &getSdFromHandle(const SD_HandleTypeDef *hsd);

} // namespace hw
