#include "io_filesystem.hpp"

namespace
{
/**
 * If mount fails we assume the filesystem is corrupted or not present, so we format it which wipes everything. Try
 * 3x to mount to make sure its not a random failure (such as from the SD card jiggling in its slot).
 */
LogFsErr tryMount(LogFs &fs, const LogFsCfg &fs_cfg)
{
    LogFsErr err;
    for (int i = 0; i < io::FileSystem::NUM_MOUNT_ATTEMPTS; i++)
    {
        err = logfs_mount(&fs, &fs_cfg);
        if (err == LOGFS_ERR_OK)
            return err;
    }
    // ReSharper disable once CppLocalVariableMightNotBeInitialized
    return err;
}

/**
 * Converts logfs errors to firmware filesystem error
 */
io::FileSystem::FileSystemError logfsErrorToFsError(const LogFsErr err)
{
    switch (err)
    {
        case LOGFS_ERR_OK:
            return io::FileSystem::FileSystemError::OK;
        case LOGFS_ERR_IO:
            return io::FileSystem::FileSystemError::ERROR_IO;
        case LOGFS_ERR_CORRUPT:
            return io::FileSystem::FileSystemError::CORRUPTED;
        case LOGFS_ERR_DNE:
            return io::FileSystem::FileSystemError::NOT_FOUND;
        case LOGFS_ERR_NOMEM:
            return io::FileSystem::FileSystemError::NO_SPACE;
        case LOGFS_ERR_INVALID_ARG:
        case LOGFS_ERR_INVALID_PATH:
        case LOGFS_ERR_NOT_OPEN:
            return io::FileSystem::FileSystemError::ERROR_BAD_ARG;
        case LOGFS_ERR_UNMOUNTED:
        case LOGFS_ERR_RD_ONLY:
        case LOGFS_ERR_WR_ONLY:
        case LOGFS_ERR_NO_MORE_FILES:
        default:
            return io::FileSystem::FileSystemError::ERROR;
    }
}
} // namespace

// Note: this code assumes that sd card is either in at the start (removable later)
// or never in at all. Furthermore, if the sd card is removed, this cannot be recovered.
namespace io
{
LogFsErr FileSystem::logfsCfgRead(const LogFsCfg *cfg, const uint32_t block, void *buf)
{
    const auto *self = static_cast<FileSystem *>(cfg->context);
    const auto  span = std::span(static_cast<uint8_t *>(buf), HW_DEVICE_SECTOR_SIZE);

    for (int i = 0; i < RETRY_COUNT; i++)
    {
        if (self->sd.read(span, block).has_value())
            return LOGFS_ERR_OK;
        if (const auto err = self->sd.abort();
            !err) // If abort fails, it's likely the SD card was removed during the read, so we can stop trying.
            break;
    }
    return LOGFS_ERR_IO;
}

LogFsErr FileSystem::logfsCfgWrite(const LogFsCfg *cfg, const uint32_t block, void *buf)
{
    const auto *self = static_cast<FileSystem *>(cfg->context);
    const auto  span = std::span(static_cast<uint8_t *>(buf), HW_DEVICE_SECTOR_SIZE);

    for (int i = 0; i < RETRY_COUNT; i++)
    {
        if (self->sd.write(span, block).has_value())
            return LOGFS_ERR_OK;

        if (const auto err = self->sd.abort();
            !err) // If abort fails, it's likely the SD card was removed during the write, so we can stop trying.
            break;
    }
    return LOGFS_ERR_IO;
}

std::expected<void, FileSystem::FileSystemError> FileSystem::init()
{
    mount_failed = false;
    LogFsErr err = tryMount(fs, fs_cfg);

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

        err = tryMount(fs, fs_cfg);
        if (err != LOGFS_ERR_OK)
        {
            mount_failed = true;
            return std::unexpected(logfsErrorToFsError(err));
        }
    }

    // Setup caches.
    files_opened.fill(false);
    for (size_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        files_cfg[i].cache = files_cache[i].data();
    }

    return {};
}

std::expected<uint32_t, FileSystem::FileSystemError> FileSystem::open(const char *path)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);

    uint32_t fd = MAX_FILE_NUMBER;
    for (uint32_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        if (!files_opened[i])
        {
            fd = i;
            break;
        }
    }

    if (fd == MAX_FILE_NUMBER)
    {
        // Couldn't find a new slot for the file.
        return std::unexpected(FileSystemError::NOT_FOUND);
    }

    files_cfg[fd].path = path;
    if (const auto err = logfs_open(&fs, &files[fd], &files_cfg[fd], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE);
        err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    files_opened[fd] = true;
    return fd;
}

std::expected<void, FileSystem::FileSystemError>
    FileSystem::read(const uint32_t fd, std::span<uint8_t> buf, const size_t size)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    uint32_t num_read;
    if (const LogFsErr err = logfs_read(&fs, &files[fd], buf.data(), size, LOGFS_READ_END, &num_read);
        err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    if (num_read != size)
        return std::unexpected(FileSystemError::ERROR);

    return {};
}

std::expected<void, FileSystem::FileSystemError>
    FileSystem::write(const uint32_t fd, const std::span<uint8_t> buf, const size_t size)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    if (const FileSystemError err = logfsErrorToFsError(logfs_write(&fs, &files[fd], buf.data(), size));
        err != FileSystemError::OK)
        return std::unexpected(err);

    return {};
}

std::expected<void, FileSystem::FileSystemError>
    FileSystem::readMetadata(const uint32_t fd, std::span<uint8_t> buf, uint32_t &num_read)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);

    if (const LogFsErr err = logfs_readMetadata(&fs, &files[fd], buf.data(), buf.size_bytes(), &num_read);
        err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    return {};
}

std::expected<void, FileSystem::FileSystemError>
    FileSystem::writeMetadata(const uint32_t fd, const std::span<const uint8_t> buf)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);

    if (const LogFsErr err = logfs_writeMetadata(&fs, &files[fd], buf.data(), buf.size_bytes()); err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    return {};
}

std::expected<void, FileSystem::FileSystemError> FileSystem::close(const uint32_t fd)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    if (const LogFsErr err = logfs_close(&fs, &files[fd]); err != LOGFS_ERR_OK)
        return std::unexpected(logfsErrorToFsError(err));

    files_opened[fd] = false;
    return {};
}

std::expected<void, FileSystem::FileSystemError> FileSystem::sync(const uint32_t fd)
{
    if (!checkMount())
        return std::unexpected(FileSystemError::MOUNT_FAILED);
    if (!checkFileDescriptor(fd))
        return std::unexpected(FileSystemError::ERROR);

    if (const FileSystemError err = logfsErrorToFsError(logfs_sync(&fs, &files[fd])); err != FileSystemError::OK)
        return std::unexpected(err);

    return {};
}
} // namespace io
