#include "io_fileSystem.h"
#include "logfs.h"
#include "hw_sd.h"
#include <string.h>

// Note: this code assumes that sd card is either in at the start (removable later)
// or never in at all. Furthermore, if the sd card is removed, this cannot be recovered.

// config
#define MAX_FILE_NUMBER 3

static LogFsErr logfsCfgRead(const LogFsCfg *cfg, uint32_t block, void *buf);
static LogFsErr logfsCfgWrite(const LogFsCfg *cfg, uint32_t block, void *buf);

// state
static uint8_t        cache[HW_DEVICE_SECTOR_SIZE];
static const LogFsCfg fs_cfg = {
    .block_count  = 1024 * 1024 * 15, // ~7.5GB
    .block_size   = HW_DEVICE_SECTOR_SIZE,
    .cache        = cache,
    .rd_only      = false,
    .read         = logfsCfgRead,
    .write        = logfsCfgWrite,
    .write_cycles = 0 // Disable wear levelling.
};
static LogFs fs;

static LogFsFileCfg files_cfg[MAX_FILE_NUMBER];
static LogFsFile    files[MAX_FILE_NUMBER];
static char         files_cache[MAX_FILE_NUMBER][HW_DEVICE_SECTOR_SIZE];
static bool         files_opened[MAX_FILE_NUMBER]; // true if the file is opened

static LogFsFile    bootcount_file;
static uint8_t      bootcount_cache[HW_DEVICE_SECTOR_SIZE];
static LogFsFileCfg bootcount_cfg = { .cache = bootcount_cache, .path = "/bootcount.txt" };

static bool ready = false;

static LogFsErr logfsCfgRead(const LogFsCfg *cfg, const uint32_t block, void *buf)
{
    UNUSED(cfg);
    if (!io_fileSystem_ready())
    {
        return LOGFS_ERR_IO;
    }

    if (hw_sd_read(buf, block, 1) != SD_CARD_OK)
    {
        return LOGFS_ERR_IO;
    }

    return LOGFS_ERR_OK;
}

static LogFsErr logfsCfgWrite(const LogFsCfg *cfg, const uint32_t block, void *buf)
{
    UNUSED(cfg);
    if (!io_fileSystem_ready())
    {
        return LOGFS_ERR_IO;
    }

    if (hw_sd_write(buf, block, 1) != SD_CARD_OK)
    {
        return LOGFS_ERR_IO;
    }

    return LOGFS_ERR_OK;
}

/**
 * Converts logfs errors to firmware filesystem error
 */
static FileSystemError logfsErrorToFsError(const LogFsErr err)
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
        case LOGFS_ERR_INVALID_ARG:
        case LOGFS_ERR_INVALID_PATH:
        case LOGFS_ERR_NOT_OPEN:
            return FILE_ERROR_BAD_ARG;
        default:
            return FILE_ERROR;
    }
}

/**
 * Allocates a file descriptor.
 * @return a valid fd
 */
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

/**
 * Checks the file descriptor to make sure it's valid, if not returns FILE_ERROR
 * @param fd file descriptor in question
 */
#define CHECK_FILE_DESCRIPTOR(fd)                               \
    if (!(fd >= 0 && fd < MAX_FILE_NUMBER && files_opened[fd])) \
        return FILE_ERROR;

FileSystemError io_fileSystem_init(void)
{
    if (!hw_sd_present())
        return FILE_ERROR_IO;
    LogFsErr err = logfs_mount(&fs, &fs_cfg);
    if (err != LOGFS_ERR_OK)
    {
        // Mounting failed meaning image is corrupted, so format.
        err = logfs_format(&fs, &fs_cfg);
        if (err != LOGFS_ERR_OK)
        {
            return logfsErrorToFsError(err);
        }

        err = logfs_mount(&fs, &fs_cfg);
        if (err != LOGFS_ERR_OK)
        {
            return logfsErrorToFsError(err);
        }
    }

    memset(files_opened, 0, sizeof(files_opened));
    for (size_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        files_cfg[i].cache = files_cache[i];
    }

    // Create/open the bootcount file.
    err = logfs_open(&fs, &bootcount_file, &bootcount_cfg, LOGFS_OPEN_CREATE | LOGFS_OPEN_RD_WR);
    if (err != LOGFS_ERR_OK)
    {
        return logfsErrorToFsError(err);
    }

    ready = true;
    return FILE_OK;
}

int io_fileSystem_open(const char *path)
{
    if (allocateFd() < 0)
    {
        return FILE_NOT_FOUND;
    }

    files_cfg[allocateFd()].path = path;
    if (logfs_open(&fs, &files[allocateFd()], &files_cfg[allocateFd()], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE) !=
        LOGFS_ERR_OK)
    {
        files_opened[allocateFd()] = false;
        return logfsErrorToFsError(
            logfs_open(&fs, &files[allocateFd()], &files_cfg[allocateFd()], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE));
    }

    return allocateFd();
}

FileSystemError io_fileSystem_read(const int fd, void *buf, const size_t size)
{
    CHECK_FILE_DESCRIPTOR(fd)
    uint32_t       num_read;
    const LogFsErr err = logfs_read(&fs, &files[fd], buf, size, LOGFS_READ_END, &num_read);
    if (err != LOGFS_ERR_OK || num_read != size)
    {
        return logfsErrorToFsError(err);
    }

    return FILE_OK;
}

FileSystemError io_fileSystem_write(const int fd, void *buf, const size_t size)
{
    CHECK_FILE_DESCRIPTOR(fd)
    return logfsErrorToFsError(logfs_write(&fs, &files[fd], buf, size));
}

uint32_t io_fileSystem_getBootCount(void)
{
    uint32_t       bootcount;
    uint32_t       num_read;
    const LogFsErr err = logfs_readMetadata(&fs, &bootcount_file, &bootcount, sizeof(bootcount), &num_read);
    if (err != LOGFS_ERR_OK || num_read != sizeof(bootcount))
    {
        bootcount = 0;
    }

    bootcount++;
    logfs_writeMetadata(&fs, &bootcount_file, &bootcount, sizeof(bootcount));
    return bootcount;
}

FileSystemError io_fileSystem_close(const int fd)
{
    CHECK_FILE_DESCRIPTOR(fd)
    const LogFsErr err = logfs_close(&fs, &files[fd]);
    if (err != LOGFS_ERR_OK)
    {
        return logfsErrorToFsError(err);
    }

    files_opened[fd] = false;
    return FILE_OK;
}

FileSystemError io_fileSystem_sync(const int fd)
{
    CHECK_FILE_DESCRIPTOR(fd)
    return logfsErrorToFsError(logfs_sync(&fs, &files[fd]));
}

bool io_fileSystem_ready()
{
    // TODO recheck ready conditions
    ready &= hw_sd_present();
    return ready;
}
