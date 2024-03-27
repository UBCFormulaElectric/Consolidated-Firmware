#include "disk.h"
#include "crc.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

#define NUM_SEQ_NUMS 4
#define INC_SEQ_NUM(pair) \
    pair->seq_num++;      \
    pair->seq_num %= NUM_SEQ_NUMS;
#define PAIR_ACTIVE_BLOCK(pair) (pair->addrs[pair->seq_num % LOGFS_PAIR_SIZE])

static inline bool disk_compareSeqNums(int s1, int s2)
{
    // Sequence numbers are compared as follows:
    // - 4 > 3
    // - 3 > 2
    // - 2 > 1
    // - 0 > 4
    return (s1 >= s2) || (s1 == 0 && s2 > 1);
}

static inline uint8_t disk_getSeqNumFromCache(const LogFs *fs)
{
    // First word (4 bytes) is reserved for CRC, next byte is for sequence number of pairs.
    uint8_t *cache_seq_num = (uint8_t *)fs->cfg->cache + sizeof(uint32_t);
    return *cache_seq_num;
}

static inline void disk_setSeqNumInCache(const LogFs *fs, uint8_t seq_num)
{
    // First word (4 bytes) is reserved for CRC, next byte is for sequence number of pairs.
    uint8_t *cache_seq_num = (uint8_t *)fs->cfg->cache + sizeof(uint32_t);
    *cache_seq_num         = seq_num;
}

static inline LogFsErr disk_replacePair(LogFs *fs, LogFsPair *pair)
{
    // Write replacement pair to head.
    LogFsPair replacement_pair;
    disk_newPair(&replacement_pair, fs->head_addr);
    RET_ERR(disk_writePair(fs, &replacement_pair, false));

    // Update old pair to point to the new one.
    RET_ERR(disk_readPair(fs, pair));
    fs->cache_pair_hdr->replacement_addr = fs->head_addr;
    RET_ERR(disk_writePair(fs, pair, false));

    // Update pair state, so pair replacements are abstracted away from the programmer.
    memcpy(pair, &replacement_pair, sizeof(LogFsPair));
    fs->head_addr += LOGFS_PAIR_SIZE;

    // Restore new pair to the cache, since it was in the cache at the start of this function and changing the cache is
    // not an obvious side-effect.
    RET_ERR(disk_readPair(fs, pair));

    return LOGFS_ERR_OK;
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

LogFsErr disk_exchangeCache(const LogFs *fs, LogFsCache *cache, uint32_t block, bool write_back, bool fetch)
{
    if (cache->cached_addr != block)
    {
        // A different block is currently in the cache, sync it to disk.
        if (write_back && cache->cached_addr != LOGFS_INVALID_BLOCK)
        {
            RET_ERR(disk_syncCache(fs, cache));
        }

        // Fetch block from disk.
        if (fetch)
        {
            RET_ERR(disk_read(fs, block, cache->buf));
        }

        // If we get here, we've successfully changed the block in the cache.
        cache->cached_addr = block;
    }

    return LOGFS_ERR_OK;
}

LogFsErr disk_syncCache(const LogFs *fs, const LogFsCache *cache)
{
    if (cache->cached_addr == LOGFS_INVALID_BLOCK)
    {
        return LOGFS_ERR_OK;
    }

    return disk_write(fs, cache->cached_addr, cache->buf);
}

void disk_newPair(LogFsPair *pair, uint32_t block)
{
    pair->addrs[0]        = block;
    pair->addrs[1]        = block + 1;
    pair->seq_num_on_disk = false;

    // Set to the highest numeric value, so the next write will increment it and wrap it to zero.
    pair->seq_num = NUM_SEQ_NUMS - 1;
}

LogFsErr disk_fetchPair(const LogFs *fs, LogFsPair *pair, uint32_t block)
{
    while (true)
    {
        // We need to figure out the active block.
        const LogFsErr block0_err     = disk_read(fs, block, fs->cfg->cache);
        const uint8_t  block0_seq_num = disk_getSeqNumFromCache(fs);
        const LogFsErr block1_err     = disk_read(fs, block + 1, fs->cfg->cache);
        const uint8_t  block1_seq_num = disk_getSeqNumFromCache(fs);

        if (!IS_ERR(block0_err) && !IS_ERR(block1_err))
        {
            // Both blocks are valid.
            if (disk_compareSeqNums(block0_seq_num, block1_seq_num))
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

        pair->addrs[0] = block;
        pair->addrs[1] = block + 1;
        RET_ERR(disk_read(fs, PAIR_ACTIVE_BLOCK(pair), fs->cfg->cache));

        if (fs->cache_pair_hdr->replacement_addr == LOGFS_INVALID_BLOCK)
        {
            // No replacement.
            break;
        }
        else
        {
            // Block has a replacement, follow the linked-list.
            block = fs->cache_pair_hdr->replacement_addr;
        }
    }

    // Successfully fetched pair from disk.
    pair->seq_num_on_disk = true;
    return LOGFS_ERR_OK;
}

LogFsErr disk_writePair(LogFs *fs, LogFsPair *pair, bool check_replace)
{
    if (!pair->seq_num_on_disk)
    {
        // Pair is new and hasn't yet been written to disk, initialize state.
        fs->cache_pair_hdr->write_cycles     = 0;
        fs->cache_pair_hdr->replacement_addr = LOGFS_INVALID_BLOCK;

        // Wipe both blocks to guarantee state is corrupted by data already on the card.
        uint8_t empty_buf[fs->cfg->block_size];
        memset(empty_buf, 0U, sizeof(empty_buf));
        RET_ERR(fs->cfg->write(fs->cfg, pair->addrs[0], empty_buf));
        RET_ERR(fs->cfg->write(fs->cfg, pair->addrs[1], empty_buf));
    }
    else if (fs->cfg->write_cycles != 0 && check_replace && fs->cache_pair_hdr->write_cycles >= fs->cfg->write_cycles)
    {
        // Pair has exhausted its write cycles, evict and replace it!
        return disk_replacePair(fs, pair);
    }
    else if (pair->seq_num % 2 == 0)
    {
        // Only increment on even writes, since write cycles should correspond to the number of writes per block (and
        // writes are evenly distributed over the 2 blocks per pair).
        fs->cache_pair_hdr->write_cycles++;
    }

    // Increment the version, and write it to the cache.
    const uint8_t prev_seq_num = pair->seq_num;
    INC_SEQ_NUM(pair);
    disk_setSeqNumInCache(fs, pair->seq_num);

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

    pair->seq_num_on_disk = true;
    return LOGFS_ERR_OK;
}

LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair)
{
    if (!pair->seq_num_on_disk)
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
            pair->seq_num = disk_getSeqNumFromCache(fs);
        }
    }

    return LOGFS_ERR_OK;
}
