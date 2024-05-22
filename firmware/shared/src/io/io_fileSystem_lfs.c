#include "lfs.h"
#include "io_fileSystem.h"
#include "io_lfsConfig.h"

#define MAX_FILE_NUMBER 3
static lfs_t lfs;

static lfs_file_t files[MAX_FILE_NUMBER];
static char       file_buffer[MAX_FILE_NUMBER][IO_LFS_CACHE_SIZE];
static bool       file_opened[MAX_FILE_NUMBER]; // true if the file is opened

static struct lfs_file_config fcfg[MAX_FILE_NUMBER];
static struct lfs_config      cfg;

static int lfsErrorToFsError(int err)
{
    switch (err)
    {
        case LFS_ERR_OK:
            return FILE_OK;
        case LFS_ERR_IO:
            return FILE_ERROR_IO;
        case LFS_ERR_CORRUPT:
            return FILE_CORRUPTED;
        case LFS_ERR_NOENT:
            return FILE_NOT_FOUND;
        case LFS_ERR_NOSPC:
            return FILE_NO_SPACE;
        default:
            return FILE_ERROR;
    }
}

FileSystemError io_fileSystem_init(void)
{
    static bool is_initialized = false;
    if (is_initialized)
    {
        return FILE_ERROR; // return error if already initialized
    }

    is_initialized = true;
    int err        = io_lfsConfig(&cfg);
    err            = lfs_mount(&lfs, &cfg);
    if (err)
    {
        lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
    }

    if (err)
    {
        return lfsErrorToFsError(err);
    }

    memset(file_opened, 0, sizeof(file_opened));
    for (size_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        fcfg[i].buffer = file_buffer[i];
    }

    return FILE_OK;
}

static int allocateFd(void)
{
    for (int i = 0; i < MAX_FILE_NUMBER; i++)
    {
        if (!file_opened[i])
        {
            file_opened[i] = true;
            return i;
        }
    }
    return FILE_NOT_FOUND;
}

static bool isValidFd(int fd)
{
    return fd >= 0 && fd < MAX_FILE_NUMBER && file_opened[fd];
}

int io_fileSystem_open(const char *path)
{
    FileSystemError err;
    int             fd = allocateFd();
    if (fd < 0)
    {
        return FILE_NOT_FOUND;
    }

    fcfg[fd].attrs->buffer = (void *)path;
    err                    = lfs_file_opencfg(
        &lfs, &files[fd], path, LFS_O_RDWR | LFS_O_CREAT,
        &fcfg[fd]); // default to open for read and write and create if not exist
    if (err)
    {
        file_opened[fd] = false;
        return FILE_ERROR;
    }

    return fd;
}

FileSystemError io_fileSystem_read(int fd, void *buffer, size_t size)
{
    if (!isValidFd(fd))
    {
        return FILE_NOT_FOUND;
    }

    FileSystemError err = lfs_file_read(&lfs, &files[fd], buffer, size);
    if (err < 0)
    {
        return lfsErrorToFsError(err);
    }

    return FILE_OK;
}

FileSystemError io_fileSystem_write(int fd, void *buffer, size_t size)
{
    if (!isValidFd(fd))
    {
        return FILE_ERROR;
    }

    int err = lfs_file_write(&lfs, &files[fd], buffer, size);
    if (err < 0)
    {
        return lfsErrorToFsError(err);
    }

    return FILE_OK;
}

uint32_t io_fileSystem_getBootCount(void)
{
    uint32_t        bootcount    = 0;
    int             bootcount_fd = io_fileSystem_open("bootcount");
    FileSystemError err          = io_fileSystem_read(bootcount_fd, &bootcount, sizeof(bootcount));
    bootcount++;

    lfs_file_rewind(&lfs, &files[bootcount_fd]);
    lfs_file_write(&lfs, &files[bootcount_fd], &bootcount, sizeof(bootcount));
    io_fileSystem_close(bootcount_fd);
    return bootcount;
}

FileSystemError io_fileSystem_close(int fd)
{
    if (!isValidFd(fd))
    {
        return FILE_ERROR;
    }

    FileSystemError err = lfs_file_close(&lfs, &files[fd]);
    if (err < 0)
    {
        return lfsErrorToFsError(err);
    }

    file_opened[fd] = false;
    return FILE_OK;
}

FileSystemError io_fileSystem_sync(int fd)
{
    if (!isValidFd(fd))
    {
        return FILE_ERROR;
    }

    FileSystemError err  = FILE_OK;
    const char     *path = fcfg[fd].attrs->buffer;

    lfs_soff_t seek = lfs_file_seek(&lfs, &files[fd], 0, LFS_SEEK_CUR); // old position

    err = lfs_file_close(&lfs, &files[fd]);
    if (err < 0)
    {
        return lfsErrorToFsError(err);
    }

    err = lfs_file_opencfg(&lfs, &files[fd], path, LFS_O_RDWR | LFS_O_CREAT, &fcfg[fd]);
    if (err < 0)
    {
        return lfsErrorToFsError(err);
    }

    lfs_file_seek(&lfs, &files[fd], seek, LFS_SEEK_SET);
    return FILE_OK;
}
