#include "disk.h"
#include "crc.h"
#include "utils.h"
#include <stdio.h>

#define NUM_SEQ_NUMS 4
#define INC_SEQ_NUM(pair) \
    pair->seq_num++;      \
    pair->seq_num %= NUM_SEQ_NUMS;
#define PAIR_ACTIVE_BLOCK(pair) (pair->blocks[pair->seq_num % LOGFS_PAIR_SIZE])

static inline bool compareSeqNums(int s1, int s2)
{
    return (s1 >= s2) || (s1 == 0 && s2 > 1);
}

static inline uint8_t getSeqNumFromCache(const LogFs *fs)
{
    // First word (4 bytes) is reserved for CRC, next byte is for sequence number of pairs.
    uint8_t *cache_seq_num = (uint8_t *)fs->cfg->cache + sizeof(uint32_t);
    return *cache_seq_num;
}

static inline void setSeqNumInCache(const LogFs *fs, uint8_t seq_num)
{
    // First word (4 bytes) is reserved for CRC, next byte is for sequence number of pairs.
    uint8_t *cache_seq_num = (uint8_t *)fs->cfg->cache + sizeof(uint32_t);
    *cache_seq_num         = seq_num;
}

inline LogFsErr disk_write(const LogFs *fs, uint32_t block, void *buf)
{
    crc_stampBlock(fs, buf);
    return fs->cfg->write(fs->cfg, block, buf);
}

inline LogFsErr disk_read(const LogFs *fs, uint32_t block, void *buf)
{
    RET_ERR(fs->cfg->read(fs->cfg, block, buf));
    return crc_checkBlock(fs, buf) ? LOGFS_ERR_OK : LOGFS_ERR_CORRUPT;
}

LogFsErr disk_changeCache(const LogFs *fs, LogFsCache *cache, uint32_t block, bool write_back, bool fetch)
{
    if (cache->block != block)
    {
        // A different block is currently in the cache, sync it to disk.
        if (write_back && cache->block != LOGFS_INVALID_BLOCK)
        {
            RET_ERR(disk_syncCache(fs, cache));
        }

        // Fetch block from disk.
        if (fetch)
        {
            RET_ERR(disk_read(fs, block, cache->buf));
        }

        cache->block = block;
    }

    return LOGFS_ERR_OK;
}

LogFsErr disk_syncCache(const LogFs *fs, const LogFsCache *cache)
{
    return disk_write(fs, cache->block, cache->buf);
}

void disk_newPair(LogFsPair *pair, uint32_t block)
{
    pair->blocks[0] = block;
    pair->blocks[1] = block + 1;
    pair->valid     = false;

    // Set to the highest numeric value, so the next write will increment it and wrap it to zero.
    pair->seq_num = NUM_SEQ_NUMS - 1;
}

LogFsErr disk_fetchPair(const LogFs *fs, LogFsPair *pair, uint32_t block)
{
    // We need to figure out the active block.
    const LogFsErr block0_err     = disk_read(fs, block, fs->cfg->cache);
    const uint8_t  block0_seq_num = getSeqNumFromCache(fs);
    const LogFsErr block1_err     = disk_read(fs, block + 1, fs->cfg->cache);
    const uint8_t  block1_seq_num = getSeqNumFromCache(fs);

    if (!IS_ERR(block0_err) && !IS_ERR(block1_err))
    {
        // Both blocks are valid.
        if (compareSeqNums(block0_seq_num, block1_seq_num))
        {
            // Block 0 is the most recent write.
            pair->seq_num = block0_seq_num;
        }
        else
        {
            // Block 1 is the most recent write.
            pair->seq_num = block1_seq_num;
        }
    }
    else if (!IS_ERR(block0_err))
    {
        // Block 0 is the only valid block.
        pair->seq_num = block0_seq_num;
    }
    else if (!IS_ERR(block1_err))
    {
        // Block 1 is the only valid block.
        pair->seq_num = block1_seq_num;
    }
    else
    {
        // Neither blocks are OK, propogate error code to caller.
        return block0_err;
    }

    pair->blocks[0] = block;
    pair->blocks[1] = block + 1;
    pair->valid     = true;
    return LOGFS_ERR_OK;
}

LogFsErr disk_writePair(const LogFs *fs, LogFsPair *pair)
{
    const uint8_t prev_seq_num = pair->seq_num;

    // Increment the version, and write it to the cache.
    INC_SEQ_NUM(pair);
    setSeqNumInCache(fs, pair->seq_num);

    // Write active block.
    LogFsErr err = disk_write(fs, PAIR_ACTIVE_BLOCK(pair), fs->cfg->cache);
    if (IS_ERR(err))
    {
        // Active block isn't valid, try the other one.
        INC_SEQ_NUM(pair);
        err = disk_write(fs, PAIR_ACTIVE_BLOCK(pair), fs->cfg->cache);

        if (IS_ERR(err))
        {
            // If both writes fail, restore pair state variables.
            pair->seq_num = prev_seq_num;
            return err;
        }
    }

    pair->valid = true;
    return LOGFS_ERR_OK;
}

LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair)
{
    if (!pair->valid)
    {
        return LOGFS_ERR_INVALID_ARG;
    }

    // Read active block.
    LogFsErr err = disk_read(fs, PAIR_ACTIVE_BLOCK(pair), fs->cfg->cache);
    if (IS_ERR(err))
    {
        // Active block isn't valid, try the other one.
        INC_SEQ_NUM(pair);
        err = disk_read(fs, PAIR_ACTIVE_BLOCK(pair), fs->cfg->cache);

        if (IS_ERR(err))
        {
            // Both blocks invalid, something's gone wrong.
            return err;
        }
        else
        {
            // Success, update sequence number from cache.
            pair->seq_num = getSeqNumFromCache(fs);
        }
    }

    return LOGFS_ERR_OK;
}
