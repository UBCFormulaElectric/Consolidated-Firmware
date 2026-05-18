#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <expected>
#include <span>

#ifdef TARGET_EMBEDDED
#include "hw_sd.hpp"
#include "logfs.h"
#elif TARGET_TEST
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#endif

namespace io
{

class FileSystem
{
  public:
    static constexpr int         NUM_MOUNT_ATTEMPTS = 3;
    static constexpr int         RETRY_COUNT        = 3;
    static constexpr uint32_t    MAX_WRITE_CYCLES   = 1000;
    static constexpr std::size_t MAX_FILE_NUMBER    = 3;
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

#ifdef TARGET_EMBEDDED
    /* Constructor*/
    explicit FileSystem(const hw::SdCard &sd_card) : fs(), sd(sd_card) {}
#else
    /* Constructor for non-embedded targets, just to make testing easier. */
    FileSystem() = default;
#endif

    /**
     * init, config, and mount the file system.
     * does not create the bootcount file
     * @return FileSystemError status
     */
    result<void> init();

    /**
     * open a file and return the file descriptor
     * Note: Don't try opening the same file twice!
     * @param path path in question
     * @return a file descriptor
     */
    result<uint32_t> open(const char *path);

    /**
     * reads from file descriptor for some size into the buffer
     * @param fd file descriptor in question
     * @param buf buffer in question
     * @param size size in question
     * @return FileSystemError status
     */
    result<void, FileSystemError> read(uint32_t fd > buf, std::size_t size);

    /**
     * writes to file descriptor for some size into the buffer
     * @param fd file descriptor in question
     * @param buf buffer in question
     * @param size size in question
     * @return FileSystemError status
     */
    result<void, FileSystemError> write(uint32_t fd > buf, std::size_t size);

    /**
     * Reads metadata from a file
     * @param fd file descriptor
     * @param buf destination buffer
     * @param num_read actual bytes read
     * @return FileSystemError status
     */
    result<void, FileSystemError> readMetadata(uint32_t fd > buf, uint32_t &num_read);

    /**
     * Writes metadata to a file
     * @param fd file descriptor
     * @param buf source buffer
     * @return FileSystemError status
     */
    result<void, FileSystemError> writeMetadata(uint32_t fd > buf);

    /**
     * Closes file given file descriptor
     * @param fd file descriptor in question
     * @return FileSystemError status
     */
    result<void> close(uint32_t fd);

    /**
     * Do the concrete write operation to the hardware
     * @param fd on this file descriptor
     * @return FileSystemError status
     */
    result<void> sync(uint32_t fd);

  private:
    bool mount_failed = false;

#ifdef TARGET_EMBEDDED
    static LogFsErr logfsCfgRead(const LogFsCfg *cfg, uint32_t block, void *buf);
    static LogFsErr logfsCfgWrite(const LogFsCfg *cfg, uint32_t block, void *buf);
    LogFsCfg        fs_cfg{
               .context      = this,
               .block_size   = HW_DEVICE_SECTOR_SIZE,
               .block_count  = 1024 * 1024 * 15, // ~7.5 GB
               .read         = logfsCfgRead,
               .write        = logfsCfgWrite,
               .cache        = cache.data(),
               .write_cycles = MAX_WRITE_CYCLES,
               .rd_only      = false,
    };
    LogFs fs;

    const hw::SdCard                                                    &sd; // sd card instance
    std::array<uint8_t, HW_DEVICE_SECTOR_SIZE>                           cache{};
    std::array<LogFsFileCfg, MAX_FILE_NUMBER>                            files_cfg{};
    std::array<LogFsFile, MAX_FILE_NUMBER>                               files{};
    std::array<std::array<char, HW_DEVICE_SECTOR_SIZE>, MAX_FILE_NUMBER> files_cache{};
#else
    std::unordered_map<size_t, std::fstream> files;
    std::unordered_set<std::string>          open_file_names;
#endif
    std::array<bool, MAX_FILE_NUMBER> files_opened{};

    /**
     * Checks the file descriptor to make sure it's valid
     * @param fd file descriptor in question
     */
    bool checkFileDescriptor(const uint32_t fd) const { return fd < MAX_FILE_NUMBER and files_opened[fd]; }

    /**
     * Checks if the filesystem has been mounted successfully.
     */
    bool checkMount() const { return !mount_failed; }
}; // class FileSystem
} // namespace io
