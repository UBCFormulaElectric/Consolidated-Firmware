#include "io_fileSystem.h"
#include "logfs.h"
#include "io_logfsConfig.h"

#define MAX_FILE_NUMBER 5
static LogFs    fs;
static LogFsCfg cfg;

static LogFsFile    files[MAX_FILE_NUMBER];
static uint8_t      files_cache[MAX_FILE_NUMBER][HW_DEVICE_SECTOR_SIZE];
static LogFsFileCfg files_cfg[MAX_FILE_NUMBER];
static bool         files_opened[MAX_FILE_NUMBER]; // True if the file is opened

static FileSystemError logfsErrorToFsError(LogFsErr err)
{
    switch (err)
    {
        case LOGFS_ERR_OK:
            return FILE_OK;
        case LOGFS_ERR_IO:
            return FILE_ERROR_IO;
        case LOGFS_ERR_CORRUPT:
            return FILE_CORRUPTED;
        case LOGFS_ERR_DNE:
            return FILE_NOT_FOUND;
        case LOGFS_ERR_NOMEM:
            return FILE_NO_SPACE;
        default:
            return FILE_ERROR;
    }
}

static int allocateFd(void)
{
    for (int i = 0; i < MAX_FILE_NUMBER; i++)
    {
        if (!files_opened[i])
        {
            files_opened[i] = true;
            return i;
        }
    }

    return -1;
}

static bool isValidFd(int fd)
{
    return fd >= 0 && fd < MAX_FILE_NUMBER && files_opened[fd];
}

int io_fileSystem_init(void)
{
    io_logfsConfig_getConfig(&cfg);

    // Force format:
    logfs_format(&fs, &cfg);

    if (logfs_mount(&fs, &cfg) != LOGFS_ERR_OK)
    {
        logfs_format(&fs, &cfg);
        const LogFsErr err = logfs_mount(&fs, &cfg);
        if (err != LOGFS_ERR_OK)
        {
            return logfsErrorToFsError(err);
        }
    }

    for (int i = 0; i < MAX_FILE_NUMBER; i++)
    {
        files_cfg[i].cache = files_cache[i];
        files_opened[i]    = false;
    }

    return FILE_OK;
}

int io_fileSystem_open(const char *path)
{
    int fd = allocateFd();
    if (fd < 0)
    {
        return FILE_NOT_FOUND;
    }

    files_cfg[fd].path = path;
    const LogFsErr err = logfs_open(&fs, &files[fd], &files_cfg[fd], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE);

    if (err != LOGFS_ERR_OK)
    {
        files_opened[fd] = false;
        return err;
    }

    return fd;
}

int io_fileSystem_write(int fd, void *buf, size_t size)
{
    if (!isValidFd(fd))
    {
        return FILE_NOT_FOUND;
    }

    return logfsErrorToFsError(logfs_write(&fs, &files[fd], buf, size));
}

int io_fileSystem_read(int fd, void *buf, size_t size)
{
    if (!isValidFd(fd))
    {
        return FILE_NOT_FOUND;
    }

    uint32_t num_read;
    return logfsErrorToFsError(logfs_read(&fs, &files[fd], buf, size, LOGFS_READ_END, &num_read));
}

int io_fileSystem_close(int fd)
{
    if (!isValidFd(fd))
    {
        return FILE_ERROR;
    }

    const LogFsErr err = logfs_close(&fs, &files[fd]);
    if (err != LOGFS_ERR_OK)
    {
        return logfsErrorToFsError(err);
    }

    files_opened[fd] = false;
    return FILE_OK;
}

uint32_t io_fileSystem_getBootCount(void)
{
    // Try opening the bootcount file.
    int fd = io_fileSystem_open("/bootcount.txt");

    uint32_t write_data = 20;

    uint32_t bootcount;
    uint32_t num_read;
    int      err = logfs_readMetadata(&fs, &files[fd], &bootcount, sizeof(uint32_t), &num_read);
    if (err != LOGFS_ERR_OK || num_read != sizeof(uint32_t))
    {
        bootcount = 1;
    }
    else
    {
        bootcount++;
    }

    err = logfs_writeMetadata(&fs, &files[fd], &bootcount, sizeof(uint32_t));
    return bootcount;
}

int io_fileSystem_sync(int fd)
{
    if (!isValidFd(fd))
    {
        return FILE_ERROR;
    }

    return logfs_sync(&fs, &files[fd]);
}
