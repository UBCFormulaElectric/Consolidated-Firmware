#include "hw_flash.hpp"
#include "hw_utils.hpp"
#include "main.h"
#include <cassert>
#include <cstdint>
#include <cstring>
#include <expected>

using namespace hw::flash;

constexpr uint8_t MAX_RETRIES = 5;

static std::expected<void, ErrorCode>
    programFlashRetry(const uint32_t address, const std::span<const std::byte> buffer);

#if defined(STM32H733xx)
constexpr uint32_t            PROGRAM_TYPE = FLASH_TYPEPROGRAM_FLASHWORD;
constexpr uint32_t            ERROR_FLAGS  = FLASH_FLAG_ALL_ERRORS_BANK1 | FLASH_FLAG_ALL_ERRORS_BANK2;
static FLASH_EraseInitTypeDef eraseStruct  = {
     .TypeErase    = FLASH_TYPEERASE_SECTORS,
     .Banks        = FLASH_BANK_1,
     .Sector       = 0,
     .NbSectors    = 1,
     .VoltageRange = FLASH_VOLTAGE_RANGE_3, // For device operating range 2.7V to 3.6V
};

#elif defined(STM32H562xx)
constexpr uint32_t            PROGRAM_TYPE     = FLASH_TYPEPROGRAM_QUADWORD;
constexpr uint32_t            ERROR_FLAGS      = FLASH_FLAG_ALL_ERRORS;
constexpr uint32_t            BANK_SECTOR_SIZE = 128U; // sectors 0–127 per bank
static FLASH_EraseInitTypeDef eraseStruct      = {
         .TypeErase = FLASH_TYPEERASE_SECTORS,
         .Banks     = FLASH_BANK_1,
         .Sector    = 0,
         .NbSectors = 1,
};
#endif

std::expected<void, ErrorCode> eraseSector(uint8_t sector)
{
#if defined(STM32H562xx)
    assert(sector < (BANK_SECTOR_SIZE * 2));
    eraseStruct.Banks = (sector < BANK_SECTOR_SIZE) ? FLASH_BANK_1 : FLASH_BANK_2; // [0, 127] Bank 1, [128, 255] Bank 2
    eraseStruct.Sector = sector % BANK_SECTOR_SIZE;
#elif defined(STM32H733xx)
    eraseStruct.Sector = sector;
#endif

    uint32_t sectorError = 0;

    HAL_FLASH_Unlock();
    HAL_StatusTypeDef halStatus = HAL_FLASHEx_Erase(&eraseStruct, &sectorError);
    HAL_FLASH_Lock();

    if (halStatus != HAL_OK || sectorError != 0xFFFFFFFFU)
    {
        return std::unexpected(ErrorCode::ERROR);
    }

    return hw_utils_convertHalStatus(halStatus);
}

std::expected<void, ErrorCode> programFlash(uint32_t address, std::span<const std::byte> buffer)
{
    return programFlashRetry(address, buffer);
}

static std::expected<void, ErrorCode> programFlashRetry(const uint32_t address, const std::span<const std::byte> buffer)
{
    std::expected<void, ErrorCode> status{ std::unexpected(ErrorCode::ERROR) };

    HAL_FLASH_Unlock();
    for (uint8_t attempt = 0; attempt < MAX_RETRIES; attempt++)
    {
        if (attempt > 0)
        {
            __HAL_FLASH_CLEAR_FLAG(ERROR_FLAGS);
        }

        status = hw_utils_convertHalStatus(HAL_FLASH_Program(
            PROGRAM_TYPE, address, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(buffer.data()))));

        if (status.has_value() &&
            (std::memcmp(reinterpret_cast<const void *>(address), buffer.data(), buffer.size()) == 0))
        {
            break;
        }
    }
    HAL_FLASH_Lock();

    return status;
}
