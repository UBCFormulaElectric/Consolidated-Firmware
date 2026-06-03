#pragma once

#include <expected>
#include "io_filesystem.hpp"

namespace app::sd
{
/**
 * Initialise the filesystem object and log the file descriptor for the file.
 * @return FileSystemError status if failure
 */
std::expected<void, io::FileSystem::FileSystemError> init_fs();

/**
 * Updates the metadata (time) of the given file descriptor with the current time from the RTC.
 * Writes seconds, minutes, hour, day, weekday, month, year (in that order) in the first 6 bytes of the metadata block.
 * @return FileSystemError status if failure
 */
std::expected<void, io::FileSystem::FileSystemError> update_metadata();

/**
 * Returns the open log file descriptor.
 */
uint32_t getLogFd();

/**
 * Returns true if the log file is currently open.
 */
bool isLogOpen();
} // namespace app::sd