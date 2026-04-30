#pragma once

#include "hw_sd.hpp"
#include "app_filesystem.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <expected>

extern "C"
{
#include "logfs.h"
}

LogFsErr logfsCfgRead(const LogFsCfg *cfg, uint32_t block, void *buf);
LogFsErr logfsCfgWrite(const LogFsCfg *cfg, uint32_t block, void *buf);

namespace io
{
enum class FileSystemError
{
    OK            = 0,
    ERROR         = -1, // general error
    CORRUPTED     = -2, // file system corrupted
    NOT_FOUND     = -3, // file not found
    NO_SPACE      = -4, // no space left
    ERROR_IO      = -5, // io error
    ERROR_BAD_ARG = -6, // Invalid argument passed in
    MOUNT_FAILED  = -7  // Initial mount failed so filesystem is unavailable for this boot cycle
};

class FileSystem
{
  public:
    static constexpr int         NUM_MOUNT_ATTEMPTS = 3;
    static constexpr int         RETRY_COUNT        = 3;
    static constexpr uint32_t    MAX_WRITE_CYCLES   = 1000;
    static constexpr std::size_t MAX_FILE_NUMBER    = app::filesystem::FileSystemContext::MAX_FILE_NUMBER;

    LogFsCfg fs_cfg;
    LogFs    fs;

    const hw::SdCard &sdCard; // sd card instance

    /* Constructor*/
    FileSystem(const hw::SdCard &sd_card, app::filesystem::FileSystemContext &ctx) : sdCard(sd_card), fs_context(ctx)
    {
        fs_cfg = LogFsCfg{
            .context      = this,
            .block_size   = HW_DEVICE_SECTOR_SIZE,
            .block_count  = 1024 * 1024 * 15, // ~7.5 GB
            .read         = logfsCfgRead,
            .write        = logfsCfgWrite,
            .cache        = cache.data(),
            .write_cycles = MAX_WRITE_CYCLES,
            .rd_only      = false,
        };
    }

    /**
     * init, config, and mount the file system
     * @return success of operation
     */
    FileSystemError init();

    /**
     * open a file and return the file descriptor
     * Note: Don't try opening the same file twice!
     * @param path path in question
     * @return a file descriptor
     */
    std::expected<void, FileSystemError> open(const char *path, uint32_t *fd);

    /**
     * reads from file descriptor for some size into the buffer
     * @param fd file descriptor in question
     * @param buf buffer in question
     * @param size size in question
     * @return FileSystemError status
     */
    std::expected<void, FileSystemError> read(uint32_t fd, void *buf, std::size_t size);

    /**
     * writes to file descriptor for some size into the buffer
     * @param fd file descriptor in question
     * @param buf buffer in question
     * @param size size in question
     * @return FileSystemError status
     */
    std::expected<void, FileSystemError> write(uint32_t fd, const void *buf, std::size_t size);
    /**
     * writes to file descriptor for some metadata
     * @param fd file descriptor in question
     * @param buf buffer in question
     * @param size size in question
     * @return FileSystemError status
     */
    std::expected<void, FileSystemError> writeMetadata(uint32_t fd, const void *buf, std::size_t size);

    /**
     * Get the bootcount of a given file system
     * @return bootcount
     */
    std::expected<void, FileSystemError> getBootCount(uint32_t *bootcount);

    /**
     * Closes file given file descriptor
     * @param fd file descriptor in question
     * @return FileSystemError status
     */
    std::expected<void, FileSystemError> close(uint32_t fd);

    /**
     * Do the concrete write operation to the hardware
     * @param fd on this file descriptor
     * @return FileSystemError status
     */
    std::expected<void, FileSystemError> sync(uint32_t fd);

    /**
     * Checks if the SD card is present.
     */
    bool isSdPresent() const;

  private:
    bool                                       mount_failed = false;
    app::filesystem::FileSystemContext        &fs_context;
    std::array<uint8_t, HW_DEVICE_SECTOR_SIZE> cache{};

    /**
     * Checks the file descriptor to make sure it's valid
     * @param fd file descriptor in question
     */
    bool checkFileDescriptor(uint32_t fd) const { return (fd < MAX_FILE_NUMBER && fs_context.files_opened[fd]); }

    /**
     * Checks if the filesystem has been mounted successfully.
     */
    bool checkMount() const { return !mount_failed; }

    /**
     * If mount fails we assume the filesystem is corrupted or not present, so we format it which wipes everything. Try
     * 3x to mount to make sure its not a random failure (such as from the SD card jiggling in its slot).
     */
    LogFsErr tryMount();

}; // class FileSystem
} // namespace io
