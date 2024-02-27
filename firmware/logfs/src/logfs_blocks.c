#include "logfs_blocks.h"
#include "logfs_util.h"
#include "logfs_crc.h"

void logfs_blocks_setType(const LogFs *fs, LogFsBlockType type)
{
    // First word is CRC, next byte is block type.
    *((uint8_t *)fs->cfg->block_cache + sizeof(uint32_t)) = (uint8_t)type;
}

LogFsBlockType logfs_blocks_getType(const LogFs *fs)
{
    // First word is CRC, next byte is block type.
    return *((uint8_t *)fs->cfg->block_cache + sizeof(uint32_t));
}

LogFsErr logfs_blocks_write(const LogFs *fs, uint32_t block, LogFsBlockType type)
{
    logfs_blocks_setType(fs, type);
    logfs_crc_stampBlock(fs);
    RET_ERR(fs->cfg->erase(fs->cfg, block));
    RET_ERR(fs->cfg->prog(fs->cfg, block, fs->cfg->block_cache));
    return LOGFS_ERR_OK;
}

LogFsErr logfs_blocks_read(const LogFs *fs, uint32_t block)
{
    RET_ERR(fs->cfg->read(fs->cfg, block, fs->cfg->block_cache));
    return logfs_crc_checkBlock(fs) ? LOGFS_ERR_OK : LOGFS_ERR_CORRUPT;
}

LogFsErr logfs_blocks_cowWrite(const LogFs *fs, uint32_t block, LogFsBlockType type)
{
    // Power-loss resilience for critical blocks is maintained by keeping a
    // copy. In the event of a failed transaction (due to power-loss or other),
    // the data can be retrieved from the copy. Which one is invalid is
    // determined via calculating a checksum.

    // Write the primary block first.
    uint32_t block_primary = block;
    RET_ERR(logfs_blocks_write(fs, block_primary, type));

    // Write the second block next.
    uint32_t block_secondary = block + 1;
    RET_ERR(logfs_blocks_write(fs, block_secondary, type));

    return LOGFS_ERR_OK;
}

LogFsErr logfs_blocks_cowRead(const LogFs *fs, uint32_t block)
{
    uint32_t block_primary   = block;
    uint32_t block_secondary = block + 1;

    if (logfs_blocks_read(fs, block_primary) == LOGFS_ERR_OK)
    {
        uint32_t   primary_crc     = logfs_crc_getFromBlock(fs);
        const bool secondary_valid = logfs_blocks_read(fs, block_secondary) == LOGFS_ERR_OK;
        bool       secondary_match = (secondary_valid && logfs_crc_getFromBlock(fs) == primary_crc);

        if (!secondary_match)
        {
            // CRCs do not match! Since the primary block gets  to
            // first, it has the most recent data, and so the secondary
            // should be updated.
            RET_ERR(logfs_blocks_read(fs, block_primary));
            RET_ERR(logfs_blocks_write(fs, block_secondary, logfs_blocks_getType(fs)));
        }
    }
    else if (logfs_blocks_read(fs, block_secondary) == LOGFS_ERR_OK)
    {
        // We already know primary is invalid, so update it to the secondary block's copy.
        RET_ERR(logfs_blocks_write(fs, block_primary, logfs_blocks_getType(fs)));
    }
    else
    {
        // Both primary and secondary copies are invalid. Corrupt!
        return LOGFS_ERR_CORRUPT;
    }

    return LOGFS_ERR_OK;
}