#pragma once

#include "io_filesystem.hpp"
#include <array>
#include <expected>
#include <cstdint>

extern "C"
{
#include "logfs.h"
}

namespace app::bootcount
{
class BootCounter
{
  public:
    /* Constructor */
    BootCounter() = default;

    /**
     * Opens/creates the bootcount file
     * @param fs initialized filesystem instance
     * @return file descriptor for bootcount.txt
     */
    std::expected<uint32_t, io::FileSystemError> init(io::FileSystem &fs);

    /**
     * Reads the current boot count from the file
     * @param fs initialized filesystem instance
     * @return current boot count on success, error otherwise
     */
    std::expected<uint32_t, io::FileSystemError> read(io::FileSystem &fs) const;

    /**
     * Increments the bootcount and writes it back to the file
     * @param fs initialized filesystem instance
     * @return updated bootcount on success, error otherwise
     */
    std::expected<uint32_t, io::FileSystemError> increment(io::FileSystem &fs) const;

  private:
    uint32_t bootcount_fd     = 0;
    bool     bootcount_inited = false;
};
} // namespace app::bootcount