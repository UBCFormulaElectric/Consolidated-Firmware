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
 * Return the result of the latest filesystem operation attempted by SoC storage.
 */
FileSystemError getStatus();

/**
 * @brief Read SoC value from SD storage.
 */
bool readSocFromSd(float &saved_soc_percent);

/**
 * @brief Write SoC value to SD storage.
 */
bool writeSocToSd(float soc_percent);

/**
 * @brief Get the last SoC value written to SD card in tenths of a percent.
 */
uint32_t getLastWrittenSocTenths();

/**
 * @brief Get the SoC value to save to SD in tenths of a percent when valid.
 */
bool convertSocToTenths(float soc_percent, uint32_t &soc_tenths);

} // namespace app::socStorage
