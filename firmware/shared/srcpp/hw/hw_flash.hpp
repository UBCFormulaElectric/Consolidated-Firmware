#pragma once

#include <span>
#include <stddef.h>
#include "hw_utils.hpp"

#if defined(STM32H733xx)
constexpr size_t WORD_BYTES = 8U * sizeof(uint32_t); // 32B
#elif defined(STM32H562xx)
constexpr size_t WORD_BYTES = 4U * sizeof(uint32_t); // 16B
#endif

namespace hw::flash
{
/**
 * @brief Erase a flash sector or page.
 * @param sector Sector to erase.
 * @return EXIT_CODE_OK on success, EXIT_CODE_ERROR on failure.
 */
std::expected<void, ErrorCode> eraseSector(uint8_t sector);

std::expected<void, ErrorCode> programFlashRetry(uint32_t address, std::span<const std::byte> buffer);

/**
 * @brief Write a contiguous buffer to flash.
          H5: 16B
          H7: 32B
 * @param address Start flash address (aligned to word size).
 * @param buffer Data span to program.
 * @return EXIT_CODE_OK if verified, EXIT_CODE_ERROR otherwise.
 */
inline std::expected<void, ErrorCode> programFlash(const uint32_t address, const std::span<const std::byte, 16> buffer)
{
    return programFlashRetry(address, buffer);
}
inline std::expected<void, ErrorCode> programFlash(const uint32_t address, const std::span<const std::byte, 32> buffer)
{
    return programFlashRetry(address, buffer);
}
} // namespace hw::flash
