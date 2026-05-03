#include "io_filesystem.hpp"

// Note: this code assumes that sd card is either in at the start (removable later)
// or never in at all. Furthermore, if the sd card is removed, this cannot be recovered.
namespace io
{
LogFsErr logfsCfgRead(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    auto *self = static_cast<io::FileSystem *>(cfg->context);

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
    auto *self = static_cast<io::FileSystem *>(cfg->context);

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
            return FileSystemError::OK;
        case LOGFS_ERR_IO:
            return FileSystemError::ERROR_IO;
        case LOGFS_ERR_CORRUPT:
            return FileSystemError::CORRUPTED;
        case LOGFS_ERR_DNE:
            return FileSystemError::NOT_FOUND;
        case LOGFS_ERR_NOMEM:
            return FileSystemError::NO_SPACE;
        case LOGFS_ERR_INVALID_ARG:
        case LOGFS_ERR_INVALID_PATH:
        case LOGFS_ERR_NOT_OPEN:
            return FileSystemError::ERROR_BAD_ARG;
        case LOGFS_ERR_UNMOUNTED:
        case LOGFS_ERR_RD_ONLY:
        case LOGFS_ERR_WR_ONLY:
        case LOGFS_ERR_NO_MORE_FILES:
            return FileSystemError::ERROR;
        default:
            return FileSystemError::ERROR;
    }
}

std::expected<void, FileSystemError> FileSystem::init(void)
{
    mount_failed = false;
    LogFsErr err = tryMount();

    if (err == LOGFS_ERR_IO)
    {
        // Failed to mount due to IO related reasons, probably because the SD card isn't plugged in.
        return std::unexpected(FileSystemError::ERROR_IO);
    }

    if (err != LOGFS_ERR_OK)
    {
        // Mounting failed meaning image is corrupted, so format.
        err = logfs_format(&fs, &fs_cfg);
        if (err != LOGFS_ERR_OK)
        {
            return std::unexpected(logfsErrorToFsError(err));
        }

        err = tryMount();
        if (err != LOGFS_ERR_OK)
        {
            mount_failed = true;
            return std::unexpected(logfsErrorToFsError(err));
        }
    }

    // Setup caches.
    files_opened.fill(false);
    for (size_t i = 0; i < FileSystem::MAX_FILE_NUMBER; i++)
    {
        files_cfg[i].cache = files_cache[i].data();
    }

    return {};
}

std::expected<uint32_t, FileSystemError> FileSystem::open(const char *path)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);

    uint32_t fd = FileSystem::MAX_FILE_NUMBER;
    for (uint32_t i = 0; i < FileSystem::MAX_FILE_NUMBER; i++)
    {
        if (!files_opened[i])
        {
            fd = i;
            break;
        }
    }

    if (fd == FileSystem::MAX_FILE_NUMBER)
    {
        // Couldn't find a new slot for the file.
        return std::unexpected(io::FileSystemError::NOT_FOUND);
    }

    files_cfg[fd].path = path;
    auto err           = logfs_open(&fs, &files[fd], &files_cfg[fd], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE);
    if (err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    files_opened[fd] = true;
    return fd;
}

std::expected<void, FileSystemError> FileSystem::read(uint32_t fd, std::span<uint8_t> buf, const size_t size)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    uint32_t num_read;
    auto     err = logfs_read(&fs, &files[fd], buf.data(), size, LOGFS_READ_END, &num_read);
    if (err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    if (num_read != size)
        return std::unexpected(io::FileSystemError::ERROR);

    return {};
}

std::expected<void, FileSystemError> FileSystem::write(uint32_t fd, std::span<uint8_t> buf, const size_t size)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    FileSystemError err = logfsErrorToFsError(logfs_write(&fs, &files[fd], buf.data(), size));
    if (err != FileSystemError::OK)
        return std::unexpected(err);

    return {};
}

std::expected<void, FileSystemError>
    FileSystem::readMetadata(uint32_t fd, std::span<uint8_t> buf, size_t size, uint32_t &num_read)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);

    auto err = logfs_readMetadata(&fs, &files[fd], buf.data(), size, &num_read);

    if (err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    return {};
}

std::expected<void, FileSystemError> FileSystem::writeMetadata(uint32_t fd, std::span<uint8_t> buf, size_t size)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);

    auto err = logfs_writeMetadata(&fs, &files[fd], buf.data(), size);

    if (err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    return {};
}

std::expected<void, FileSystemError> FileSystem::close(uint32_t fd)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    auto err = logfs_close(&fs, &files[fd]);
    if (err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    files_opened[fd] = false;
    return {};
}

std::expected<void, FileSystemError> FileSystem::sync(uint32_t fd)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    FileSystemError err = logfsErrorToFsError(logfs_sync(&fs, &files[fd]));
    if (err != FileSystemError::OK)
        return std::unexpected(err);

    return {};
}

bool FileSystem::isSdPresent() const
{
    return sdCard.sdPresent();
}
} // namespace io
