#pragma once

#include "logfs2.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _Disk
    {
        // User-provided context used to send extra data to the I/O functions.
        void *context;
        // Number of contiguous blocks available to the filesystem.
        uint32_t block_count;
        // Read a block at a given address.
        LogFsErr (*read)(const struct _Disk *disk, BlockAddr addr, BlockBuf buf);
        // Program a block at a given address. Will be erased beforehand.
        LogFsErr (*write)(const struct _Disk *disk, BlockAddr addr, BlockBuf buf);
        // If the entire filesystem should be marked as read-only.
        bool rd_only;
    } Disk;

    /**
     * Write data to a block on the disk.
     *
     * @param fs Disk object.
     * @param block Block address to write to.
     * @param buf 1-block sized buffer to write from.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_write(const Disk *disk, BlockAddr addr, BlockBuf buf);

    /**
     * Read data from a block on the disk.
     *
     * @param fs Filesystem object.
     * @param block Block address to read from.
     * @param buf 1-block sized buffer to read to.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_read(const Disk *disk, BlockAddr addr, BlockBuf buf);

#ifdef __cplusplus
}
#endif
