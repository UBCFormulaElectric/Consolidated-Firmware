#include "io_filesystem.hpp"
#include "hw_sd.hpp"

extern "C" {
#include "logfs.h"
}

#include <cstring>

// Note: this code assumes that sd card is either in at the start (removable later)
// or never in at all. Furthermore, if the sd card is removed, this cannot be recovered.

namespace io
{
LogFsErr logfsCfgRead(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    auto *self = static_cast<io::FileSystem*>(cfg->context);

    auto span = std::span<uint8_t>(static_cast<uint8_t *>(buf), HW_DEVICE_SECTOR_SIZE);

    for (int i = 0; i < io::FileSystem::RETRY_COUNT; i++)
    {
        if (self->sdCard.read(span, block).has_value())
            return LOGFS_ERR_OK;

        self->sdCard.abort();
    }
    return LOGFS_ERR_IO;
}

LogFsErr logfsCfgWrite(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    auto *self = static_cast<io::FileSystem*>(cfg->context);

    auto span = std::span<uint8_t>(static_cast<uint8_t *>(buf), HW_DEVICE_SECTOR_SIZE);

    for (int i = 0; i < io::FileSystem::RETRY_COUNT; i++)
    {
        if (self->sdCard.write(span, block).has_value())
            return LOGFS_ERR_OK;

        self->sdCard.abort();
    }
    return LOGFS_ERR_IO;
}

/**
 * Converts logfs errors to firmware filesystem error
 */
FileSystemError logfsErrorToFsError(const LogFsErr err)
{
    switch (err)
    {
        case LOGFS_ERR_OK:
            return io::FileSystemError::FILE_OK;
        case LOGFS_ERR_IO:
            return io::FileSystemError::FILE_ERROR_IO;
        case LOGFS_ERR_CORRUPT:
            return io::FileSystemError::FILE_CORRUPTED;
        case LOGFS_ERR_DNE:
            return io::FileSystemError::FILE_NOT_FOUND;
        case LOGFS_ERR_NOMEM:
            return io::FileSystemError::FILE_NO_SPACE;
        case LOGFS_ERR_INVALID_ARG:
        case LOGFS_ERR_INVALID_PATH:
        case LOGFS_ERR_NOT_OPEN:
            return io::FileSystemError::FILE_ERROR_BAD_ARG;
        // FOR THE FOUR BELOW, IDK WHAT TO RETURN
        case LOGFS_ERR_UNMOUNTED:
        case LOGFS_ERR_RD_ONLY:
        case LOGFS_ERR_WR_ONLY:
        case LOGFS_ERR_NO_MORE_FILES:
            // END HERE
        default:
            return io::FileSystemError::FILE_ERROR;
    }
}

LogFsErr FileSystem::tryMount()
{
    LogFsErr err;

    for (int i = 0; i < NUM_MOUNT_ATTEMPTS; i++)
    {
        err = logfs_mount(&fs, &fs_cfg);
        if (err == LOGFS_ERR_OK)
            return err;
    }

    return err;
}

FileSystemError FileSystem::init(void)
{
    mount_failed = false;
    LogFsErr err = tryMount();

    if (err == LOGFS_ERR_IO)
    {
        // Failed to mount due to IO related reasons, probably because the SD
        // card isn't plugged in.
        return io::FileSystemError::FILE_ERROR_IO;
    }

    if (err != LOGFS_ERR_OK)
    {
        // Mounting failed meaning image is corrupted, so format.
        err = logfs_format(&fs, &fs_cfg);
        if (err != LOGFS_ERR_OK)
        {
            return logfsErrorToFsError(err);
        }

        err = tryMount();
        if (err != LOGFS_ERR_OK)
        {
            mount_failed = true;
            return logfsErrorToFsError(err);
        }
    }

    // Setup caches.
    files_opened.fill(false);
    for (size_t i = 0; i < FileSystem::MAX_FILE_NUMBER; i++)
    {
        files_cfg[i].cache = files_cache[i].data();
    }

    // Create/open the bootcount file.
    return logfsErrorToFsError(logfs_open(&fs, &bootcount_file, &bootcount_cfg, LOGFS_OPEN_CREATE | LOGFS_OPEN_RD_WR));
}

FileSystemError FileSystem::open(const char *path, uint32_t *fd)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;

    for (uint32_t i = 0; i < FileSystem::MAX_FILE_NUMBER; i++)
    {
        if (!files_opened[i])
        {
            *fd = i;
            break;
        }
    }

    if (*fd == FileSystem::MAX_FILE_NUMBER)
    {
        // Couldn't find a new slot for the file.
        return io::FileSystemError::FILE_NOT_FOUND;
    }

    files_cfg[*fd].path = path;
    auto err = logfs_open(&fs, &files[*fd], &files_cfg[*fd], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE);
    if (err != LOGFS_ERR_OK)
        return logfsErrorToFsError(err);

    files_opened[*fd] = true;
    return io::FileSystemError::FILE_OK;
}

FileSystemError FileSystem::read(uint32_t fd, void *buf, const size_t size)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;
    if (!checkFileDescriptor(fd))
        return FileSystemError::FILE_ERROR;

    uint32_t num_read;
    auto err = logfs_read(&fs, &files[fd], buf, size, LOGFS_READ_END, &num_read);
    if (err != LOGFS_ERR_OK)
        return logfsErrorToFsError(err);

    if (num_read != size)
    {
        return io::FileSystemError::FILE_ERROR;
    }

    return io::FileSystemError::FILE_OK;
}

FileSystemError FileSystem::write(uint32_t fd, const void *buf, const size_t size)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;
    if (!checkFileDescriptor(fd))
        return FileSystemError::FILE_ERROR;

    return logfsErrorToFsError(logfs_write(&fs, &files[fd], buf, size));
}

FileSystemError FileSystem::writeMetadata(uint32_t fd, const void *buf, size_t size)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;
    if (!checkFileDescriptor(fd))
        return FileSystemError::FILE_ERROR;

    return logfsErrorToFsError(logfs_writeMetadata(&fs, &files[fd], buf, size));
}

FileSystemError FileSystem::getBootCount(uint32_t *bootcount)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;

    uint32_t num_read;
    auto err = logfs_readMetadata(&fs, &bootcount_file, bootcount, sizeof(uint32_t), &num_read);
    if (err != LOGFS_ERR_OK)
        return logfsErrorToFsError(err);

    if (num_read != sizeof(*bootcount))
    {
        *bootcount = 0;
    }

    (*bootcount)++;
    return logfsErrorToFsError(logfs_writeMetadata(&fs, &bootcount_file, bootcount, sizeof(*bootcount)));
}

FileSystemError FileSystem::close(uint32_t fd)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;
    if (!checkFileDescriptor(fd))
        return FileSystemError::FILE_ERROR;

    auto err = logfs_close(&fs, &files[fd]);
    if (err != LOGFS_ERR_OK)
        return logfsErrorToFsError(err);

    files_opened[fd] = false;
    return io::FileSystemError::FILE_OK;
}

FileSystemError FileSystem::sync(uint32_t fd)
{
    if (!checkMount())
        return FileSystemError::FILE_MOUNT_FAILED;
    if (!checkFileDescriptor(fd))
        return FileSystemError::FILE_ERROR;

    return logfsErrorToFsError(logfs_sync(&fs, &files[fd]));
}

bool FileSystem::isSdPresent() const
{
    return sdCard.sdPresent();
}
} // namespace io
