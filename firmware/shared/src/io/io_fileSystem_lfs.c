#include "lfs.h"
#include "io_fileSystem.h"
#include "io_lfs_config.h"

#define LFS_NO_MALLOC 1
#define MAX_FILE_NUMBER 5
static lfs_t lfs;

static lfs_file_t files[MAX_FILE_NUMBER];
static char       file_buffer[MAX_FILE_NUMBER][IO_LFS_CACHE_SIZE];
static bool       file_opened[MAX_FILE_NUMBER]; // true if the file is opened

static struct lfs_file_config fcfg[MAX_FILE_NUMBER];
static struct lfs_config      cfg;

int io_fileSystem_init(void)
{
    int err = io_lfs_config(&cfg);
    err     = lfs_mount(&lfs, &cfg);
    if (err)
    {
        lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
    }

    if (err)
    {
        return err;
    }

    memset(file_opened, 0, sizeof(file_opened));
    for (size_t i = 0; i < MAX_FILE_NUMBER; i++)
    {
        fcfg[i].buffer = file_buffer[i];
    }

    return 0;
}

static int allocate_file_number(void)
{
    for (int i = 0; i < MAX_FILE_NUMBER; i++)
    {
        if (!file_opened[i])
        {
            file_opened[i] = true;
            return i;
        }
    }
    return -1;
}

static bool is_valid_fd(int fd)
{
    return fd >= 0 && fd < MAX_FILE_NUMBER && file_opened[fd];
}

int io_fileSystem_open(const char *path)
{
    int err;
    int fd = allocate_file_number();
    if (fd < 0)
    {
        return -1;
    }
    fcfg->attrs->buffer = (void *)path;

    err = lfs_file_opencfg(
        &lfs, &files[fd], path, LFS_O_RDWR | LFS_O_CREAT,
        &fcfg[fd]); // default to open for read and write and create if not exist
    if (err)
    {
        file_opened[fd] = false;
        return FILE_ERROR;
    }

    return fd;
}

int io_fileSystem_read(int fd, void *buffer, size_t size)
{
    if (!is_valid_fd(fd))
    {
        return FILE_ERROR;
    }
    int err = lfs_file_read(&lfs, &files[fd], buffer, size);
    if (err < 0)
    {
        return FILE_ERROR;
    }
    return FILE_OK;
}

int io_fileSystem_write(int fd, void *buffer, size_t size)
{
    if (!is_valid_fd(fd))
    {
        return FILE_ERROR;
    }
    int err = lfs_file_write(&lfs, &files[fd], buffer, size);
    if (err < 0)
    {
        return FILE_ERROR;
    }
    return FILE_OK;
}

uint32_t io_fileSystem_getBootCount(void)
{
    uint32_t bootcount    = 0;
    int      bootcount_fd = io_fileSystem_open("bootcount");
    int      err          = io_fileSystem_read(bootcount_fd, &bootcount, sizeof(bootcount));
    bootcount++;

    lfs_file_rewind(&lfs, &files[bootcount_fd]);
    lfs_file_write(&lfs, &files[bootcount_fd], &bootcount, sizeof(bootcount));
    io_fileSystem_close(bootcount_fd);
    return bootcount;
}

int io_fileSystem_close(int fd)
{
    if (!is_valid_fd(fd))
    {
        return FILE_ERROR;
    }
    int err = lfs_file_close(&lfs, &files[fd]);
    if (err < 0)
    {
        return FILE_ERROR;
    }
    file_opened[fd] = false;
    return FILE_OK;
}

int io_fileSystem_sync(int fd)
{
    if (!is_valid_fd(fd))
    {
        return FILE_ERROR;
    }

    const char *path = fcfg[fd].attrs->buffer;

    lfs_soff_t seek = lfs_file_seek(&lfs, &files[fd], 0, LFS_SEEK_CUR);

    lfs_file_close(&lfs, &files[fd]);
    lfs_file_opencfg(&lfs, &files[fd], path, LFS_O_RDWR | LFS_O_CREAT, &fcfg[fd]);
    lfs_file_seek(&lfs, &files[fd], seek, LFS_SEEK_SET);
    return FILE_OK;
}
