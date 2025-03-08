#include "disk.h"
// #include "utils.h"

LogFsErr disk_write(const Disk *disk, BlockAddr addr, BlockBuf buf)
{
    // crc_stampBlock(fs, buf);
    return disk->write(disk, addr, buf);
}

LogFsErr disk_read(const Disk *disk, BlockAddr addr, BlockBuf buf)
{
    // RET_ERR(disk->read(disk, addr, buf));
    // return crc_checkBlock(fs, buf) ? LOGFS_ERR_OK : LOGFS_ERR_CORRUPT;
    return disk->read(disk, addr, buf);
}
