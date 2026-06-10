#pragma once

#include <expected>
#include "io_filesystem.hpp"
#include "util_errorCodes.hpp"

namespace app::sd
{
/**
 * Initialise the filesystem object and log the file descriptor for the file.
 * @return FileSystemError status if failure
 */
std::expected<void, io::FileSystem::FileSystemError> init_fs();

/**
 * Updates the metadata of the given file descriptor with the corrected DAM basetime.
 * Writes seconds, minutes, hour, day, weekday, month, year (in that order) in the first 7 bytes of the metadata block.
 * Boot files are created with a hashed suffix, e.g. "/boot_1_ab12cd34.bin".
 * @return ErrorCode status if failure
 */
std::expected<void, ErrorCode> update_metadata();

/**
 * Request a metadata rewrite / log sync from another task. The work is deferred to
 * service(), which runs on TaskLogging (the sole SD owner), so callers never touch the SD driver.
 */
void requestMetadataUpdate();
void requestSync();

/**
 * Run any pending requests. Must only be called from TaskLogging.
 */
void service();

/**
 * Returns the open log file descriptor.
 */
uint32_t getLogFd();

/**
 * Returns true if the log file is currently open.
 */
bool isLogOpen();
} // namespace app::sd