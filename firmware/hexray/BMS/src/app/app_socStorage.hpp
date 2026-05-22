#pragma once

#include <cstdint>
#include <expected>

extern "C"
{
#include "io_fileSystem.h"
}

namespace app::socStorage
{

/**
 * Initialize SoC persistence storage using the filesystem layer.
 */
void init();

/**
 * Return whether or not SoC persistence storage is available.
 */
bool isAvailable();

/**
 * @brief Read SoC value from SD storage.
 */
std::expected<float, FileSystemError> readSocFromSd();

/**
 * @brief Write SoC value to SD storage.
 */
std::expected<void, FileSystemError> writeSocToSd(float soc_percent);

/**
 * @brief Get the last SoC value written to SD card in tenths of a percent.
 */
uint32_t getLastWrittenSocTenths();

/**
 * @brief Get the SoC value to save to SD in tenths of a percent when valid.
 */
std::expected<uint32_t, ErrorCode> convertSocToTenths(float soc_percent);

} // namespace app::socStorage
