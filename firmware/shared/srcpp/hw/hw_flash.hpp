#pragma once

#include <cstdint>
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
class Flash
{
  public:
    /**
     * @brief Erase a flash sector or page.
     * @param sector Sector to erase.
     * @return EXIT_CODE_OK on success, EXIT_CODE_ERROR on failure.
     */
    ExitCode eraseSector(uint8_t sector);

    /**
     * @brief Write a contiguous buffer to flash.
              H5: 16B
              H7: 32B
     * @param buffer_size the size of buffer, either 16B or 32B.
     * @param address Start flash address (aligned to word size).
     * @param buffer Data span to program.
     * @return EXIT_CODE_OK if verified, EXIT_CODE_ERROR otherwise.
     */
    ExitCode programFlash(uint32_t address, std::span<const std::byte, 16> buffer);
    ExitCode programFlash(uint32_t address, std::span<const std::byte, 32> buffer);

  private:
    static ExitCode programFlashRetry(const uint32_t address, const std::span<const std::byte> buffer);
};
} // namespace hw::flash
