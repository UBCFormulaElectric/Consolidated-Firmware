#include "disk.h"
#include "crc.h"
#include "utils.h"

static inline LogFsErr disk_writeCache(const LogFs *fs, uint32_t block)
{
    crc_stampBlock(fs);
    RET_ERR(fs->cfg->write(fs->cfg, block, fs->cfg->cache));
}

static inline LogFsErr disk_readCache(const LogFs *fs, uint32_t block)
{
    RET_ERR(fs->cfg->read(fs->cfg, block, fs->cfg->cache));
    return crc_checkBlock(fs) ? LOGFS_ERR_OK : LOGFS_ERR_CORRUPT;
}

static inline LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair, uint32_t block)
{
    LogFsErr err;

    if (pair->init)
    {
        // Read active block.
        if (CHECK_ERR(disk_readCache(fs, pair->blocks[pair->active])))
        {
            // Active block isn't valid, try the other one.
            pair->active++;
            pair->active %= 2;

            if (disk_readCache(fs, pair->blocks[pair->active]))
            {
                // Both blocks invalid, filesystem is corrupt.
                return LOGFS_ERR_CORRUPT;
            }
        }
    }
    else
    {
        // disk_readCache(fs, pair->blocks[pair->active])

        pair->init = true;
    }
    /*
    if init:
        read active
        if fail:
            change active
            read active
            if fail:
                return corrupt
    else:
        read first
        read second
        if one valid:
            set it as active
        if none valid:
            return corrupt
        if both valid:
            pick most significant ID
    */
}