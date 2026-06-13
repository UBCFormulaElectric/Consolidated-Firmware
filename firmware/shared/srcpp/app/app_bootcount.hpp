#pragma once

#include "io_filesystem.hpp"
#include <expected>

namespace app::bootcount
{
/**
 * Updates the bootcount file with the current bootcount value.
 * If the file doesn't exist, it will be created and initialized to 1.
 * @param fs initialized filesystem instance
 * @return the boot number written to the bootcount file
 */
std::expected<uint32_t, io::FileSystem::FileSystemError> update(io::FileSystem &fs);
} // namespace app::bootcount