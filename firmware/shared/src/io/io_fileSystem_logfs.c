#include "io_fileSystem.h"
#include "logfs.h"
#include "hw_sd.h"
#include "io_time.h"
#include <string.h>

// Note: this code assumes that sd card is either in at the start (removable later)
// or never in at all. Furthermore, if the sd card is removed, this cannot be recovered.

// config
#define MAX_FILE_NUMBER 3
#define NUM_MOUNT_ATTEMPTS 3
#define RETRY_COUNT 3
#define RETRY_DELAY_MS 50

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

static bool mount_failed = false;

static LogFsErr logfsCfgRead(const LogFsCfg *cfg, const uint32_t block, void *buf)
{
    UNUSED(cfg);

    int retry_count = RETRY_COUNT;

    while (hw_sd_read(buf, block, 1) != SD_CARD_OK && retry_count > 0)
    {
        retry_count--;
        hw_sd_abort();
    }

    if (retry_count == 0)
    {
        return LOGFS_ERR_IO;
    }

    return LOGFS_ERR_OK;
}

static LogFsErr logfsCfgWrite(const LogFsCfg *cfg, const uint32_t block, void *buf)
{
    UNUSED(cfg);

    int retry_count = RETRY_COUNT;
    while (hw_sd_write(buf, block, 1) != SD_CARD_OK && retry_count > 0)
    {
        retry_count--;
        hw_sd_abort(); // Abort the current operation to retry
    }
    if (retry_count == 0)
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
 * If mount fails we assume the filesystem is corrupted or not present, so we format it which wipes everything. Try 3x
 * to mount to make sure its not a random failure (such as from the SD card jiggling in its slot).
 */
static LogFsErr tryMount(void)
{
    LogFsErr err;
    for (int i = 0; i < NUM_MOUNT_ATTEMPTS; i++)
    {
        err = logfs_mount(&fs, &fs_cfg);
        if (err == LOGFS_ERR_OK)
        {
            break;
        }
    }

    return err;
}

/**
 * Checks the file descriptor to make sure it's valid, if not returns FILE_ERROR
 * @param fd file descriptor in question
 */
#define CHECK_FILE_DESCRIPTOR(fd)                    \
    if (!(fd < MAX_FILE_NUMBER && files_opened[fd])) \
        return FILE_ERROR;

/**
 * Checks if the filesystem has been mounted successfully.
 */
#define CHECK_MOUNT() \
    if (mount_failed) \
        return FILE_MOUNT_FAILED;

/**
 * Checks if the given expression evaluates to an error, and return it if so.
 * @param f Expression to evaluate
 */
#define CHECK_ERR(f)                     \
    const LogFsErr err = f;              \
    if (err != LOGFS_ERR_OK)             \
    {                                    \
        return logfsErrorToFsError(err); \
    }

FileSystemError io_fileSystem_init(void)
{
    mount_failed = false;
    LogFsErr err = tryMount();

    if (err == LOGFS_ERR_IO)
    {
        // Failed to mount due to IO related reasons, probably because the SD
        // card isn't plugged in.
        return FILE_ERROR_IO;
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
    memset(files_opened, 0, sizeof(files_opened));
    for (size_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        files_cfg[i].cache = files_cache[i];
    }

    // Create/open the bootcount file.
    return logfsErrorToFsError(logfs_open(&fs, &bootcount_file, &bootcount_cfg, LOGFS_OPEN_CREATE | LOGFS_OPEN_RD_WR));
}

FileSystemError io_fileSystem_open(const char *path, uint32_t *fd)
{
    CHECK_MOUNT();

    for (uint32_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        if (!files_opened[i])
        {
            *fd = i;
            break;
        }
    }

    if (*fd == MAX_FILE_NUMBER)
    {
        // Couldn't find a new slot for the file.
        return FILE_NOT_FOUND;
    }

    files_cfg[*fd].path = path;
    CHECK_ERR(logfs_open(&fs, &files[*fd], &files_cfg[*fd], LOGFS_OPEN_RD_WR | LOGFS_OPEN_CREATE));

    files_opened[*fd] = true;
    return FILE_OK;
}

FileSystemError io_fileSystem_read(uint32_t fd, void *buf, const size_t size)
{
    CHECK_MOUNT();
    CHECK_FILE_DESCRIPTOR(fd);

    uint32_t num_read;
    CHECK_ERR(logfs_read(&fs, &files[fd], buf, size, LOGFS_READ_END, &num_read));

    if (num_read != size)
    {
        return FILE_ERROR;
    }

    return FILE_OK;
}

FileSystemError io_fileSystem_write(uint32_t fd, void *buf, const size_t size)
{
    CHECK_MOUNT();
    CHECK_FILE_DESCRIPTOR(fd);

    return logfsErrorToFsError(logfs_write(&fs, &files[fd], buf, size));
}

FileSystemError io_fileSystem_getBootCount(uint32_t *bootcount)
{
    CHECK_MOUNT();

    uint32_t num_read;
    CHECK_ERR(logfs_readMetadata(&fs, &bootcount_file, bootcount, sizeof(uint32_t), &num_read));

    if (num_read != sizeof(bootcount))
    {
        *bootcount = 0;
    }

    (*bootcount)++;
    return logfsErrorToFsError(logfs_writeMetadata(&fs, &bootcount_file, bootcount, sizeof(bootcount)));
}

FileSystemError io_fileSystem_close(uint32_t fd)
{
    CHECK_MOUNT();
    CHECK_FILE_DESCRIPTOR(fd);

    CHECK_ERR(logfs_close(&fs, &files[fd]));

    files_opened[fd] = false;
    return FILE_OK;
}

FileSystemError io_fileSystem_sync(uint32_t fd)
{
    CHECK_MOUNT();
    CHECK_FILE_DESCRIPTOR(fd);

    return logfsErrorToFsError(logfs_sync(&fs, &files[fd]));
}

bool io_fileSystem_present(void)
{
    return hw_sd_present();
}
