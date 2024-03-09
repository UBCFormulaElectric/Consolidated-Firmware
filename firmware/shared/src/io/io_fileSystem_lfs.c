#include "lfs.h"
#include "io_lfs.h"

#define MAX_FILE_NUMBER 5
static lfs_t lfs;

static lfs_file_t files[MAX_FILE_NUMBER];
static char       file_buffer[MAX_FILE_NUMBER][IO_LFS_CACHE_SIZE];
static bool       file_opened[MAX_FILE_NUMBER]; // true if the file is opened

static struct lfs_config cfg[MAX_FILE_NUMBER];

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
        cfg[i].buffer = file_buffer[i];
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

int io_fileSystem_open(const char *path, int flags)
{
    int err;
    int fd = allocate_file_number();
    if (fd < 0)
    {
        return -1;
    }

    err = lfs_file_opencfg(&lfs, &files[fd], path, flags, &fcfg[fd]);
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

int io_fileSystem_write(int fd, const void *buffer, size_t size)
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

    lfs_soff_t  seek  = lfs_file_seek(&lfs, &file[fd], 0, LFS_SEEK_CUR);
    const char *path  = file[fd].path;
    const int   flags = file[fd].flags;

    lfs_file_close(&lfs, &file[fd]);
    lfs_file_opencfg(&lfs, &file[fd], path, flags, &fcfg[fd]);
    lfs_file_seek(&lfs, &file[fd], seek, LFS_SEEK_SET);
}
