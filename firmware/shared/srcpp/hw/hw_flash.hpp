#pragma once

#include <cstdint>
#include <span>
#include "hw_utils.hpp"

namespace hw::flash
{

#if defined(STM32F412Rx)
enum class ProgramType : uint32_t
{
    Byte     = FLASH_TYPEPROGRAM_BYTE,
    HalfWord = FLASH_TYPEPROGRAM_HALFWORD,
    Word     = FLASH_TYPEPROGRAM_WORD
};

#elif defined(STM32H733xx)
enum class ProgramType : uint32_t
{
    FlashWord = FLASH_TYPEPROGRAM_FLASHWORD
};

#elif defined(STM32H562xx)
enum class ProgramType : uint32_t
{
    QuadWord = FLASH_TYPEPROGRAM_QUADWORD
};
#endif

class Flash
{
  public:
    /**
     * @brief Erase a flash sector or page.
     * @param eraseStruct HAL erase struct with configured bank, type, and index.
     * @return EXIT_CODE_OK on success, EXIT_CODE_ERROR on failure.
     */
    static ExitCode eraseSector(FLASH_EraseInitTypeDef &eraseStruct);

    /**
     * @brief Write a 32-bit word to flash.
     * @param address Flash address.
     * @param value 32-bit data to write.
     * @param type Programming mode (Word).
     * @return EXIT_CODE_OK if verified, EXIT_CODE_ERROR otherwise.
     */
    static ExitCode programWord(const uint32_t address, const uint32_t value, const ProgramType type);

    /**
     * @brief Write a contiguous buffer to flash.
              H7: 32B
              H5: 16B
     * @param address Start flash address (aligned to word size).
     * @param buffer Data span to program.
     * @param type Programming mode (FlashWord or QuadWord).
     * @return EXIT_CODE_OK if verified, EXIT_CODE_ERROR otherwise.
     */
    static ExitCode
        programBuffer(const uint32_t address, const std::span<const std::byte> buffer, const ProgramType type);

  private:
    static constexpr int MAX_RETRIES = 5;
    /**
     * @brief Perform flash programming with retries and verification.
     * @param address Flash address to write.
     * @param bytes Data bytes to program.
     * @param type Flash program type.
     * @return EXIT_CODE_OK on success, EXIT_CODE_ERROR after retries fail.
     */
    static ExitCode
        programRetry(const uint32_t address, const std::span<const std::byte> bytes, const ProgramType type);
};
} // namespace hw::flash
