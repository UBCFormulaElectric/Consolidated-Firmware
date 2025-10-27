#include "hw_flash.hpp"
#include "hw_utils.hpp"
#include "main.h"
#include <cstring>

using namespace hw::flash;

#if defined(STM32F412Rx)
constexpr uint32_t ERROR_FLAGS =
    FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_OPERR | FLASH_FLAG_RDERR;

#elif defined(STM32H733xx)
constexpr uint32_t ERROR_FLAGS = FLASH_FLAG_ALL_ERRORS_BANK1 | FLASH_FLAG_ALL_ERRORS_BANK2;
constexpr size_t   WORD_BYTES  = 8U * sizeof(uint32_t); // 32B

#elif defined(STM32H562xx)
constexpr uint32_t ERROR_FLAGS = FLASH_FLAG_ALL_ERRORS;
constexpr size_t   WORD_BYTES  = 4U * sizeof(uint32_t); // 16B
#endif

ExitCode Flash::eraseSector(FLASH_EraseInitTypeDef &eraseStruct)
{
    uint32_t sectorError = 0;
    HAL_FLASH_Unlock();
    const ExitCode status = hw_utils_convertHalStatus(HAL_FLASHEx_Erase(&eraseStruct, &sectorError));
    HAL_FLASH_Lock();

    return status == ExitCode::EXIT_CODE_OK && sectorError == 0xFFFFFFFFU ? ExitCode::EXIT_CODE_OK
                                                                          : ExitCode::EXIT_CODE_ERROR;
}

ExitCode Flash::programWord(const uint32_t address, const uint32_t value, const ProgramType type)
{
    const std::span<const std::byte> bytes{ reinterpret_cast<const std::byte *>(&value), sizeof(value) };

    HAL_FLASH_Unlock();
    const ExitCode ok = programRetry(address, bytes, type);
    HAL_FLASH_Lock();
    return ok;
}

ExitCode Flash::programBuffer(const uint32_t address, const std::span<const std::byte> buffer, const ProgramType type)
{
    HAL_FLASH_Unlock();
    const ExitCode ok = programRetry(address, buffer, type);
    HAL_FLASH_Lock();
    return ok;
}

ExitCode Flash::programRetry(const uint32_t address, const std::span<const std::byte> bytes, const ProgramType type)
{
    ExitCode status = ExitCode::EXIT_CODE_BUSY;

    for (uint8_t attempt = 0; attempt < MAX_RETRIES; attempt++)
    {
        if (attempt > 0)
        {
            __HAL_FLASH_CLEAR_FLAG(ERROR_FLAGS);
        }

        status = hw_utils_convertHalStatus(
            HAL_FLASH_Program(static_cast<uint32_t>(type), address, reinterpret_cast<uint32_t>(bytes.data())));

        if ((status == ExitCode::EXIT_CODE_OK) &&
            (std::memcmp(reinterpret_cast<const void *>(address), bytes.data(), bytes.size()) == 0))
        {
            break;
        }
    }

    return status;
}
