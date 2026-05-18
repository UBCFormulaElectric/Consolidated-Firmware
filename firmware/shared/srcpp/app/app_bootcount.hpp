#pragma once

#include "io_filesystem.hpp"
#include <expected>

namespace app::bootcount
{
/**
 * Updates the bootcount file with the current bootcount value.
 * If the file doesn't exist, it will be created and initialized to 1.
 * @param fs initialized filesystem instance
 */
result<void> update(io::FileSystem &fs);
} // namespace app::bootcount