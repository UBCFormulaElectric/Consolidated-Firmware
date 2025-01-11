#pragma once

#include "logfs.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Write data to a block on the disk.
     *
     * @param fs Filesystem object.
     * @param block Block address to write to.
     * @param buf 1-block sized buffer to write from.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_write(const LogFs *fs, uint32_t block, void *buf);

    /**
     * Read data from a block on the disk.
     *
     * @param fs Filesystem object.
     * @param block Block address to read from.
     * @param buf 1-block sized buffer to read to.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_read(const LogFs *fs, uint32_t block, void *buf);

    /**
     * Change the block that currently exists in a 1-block sized cache. If the new block doesn't match the old block:
     * 1. Cached data will be synced to disk if `write_back` is true.
     * 2. New block will be fetched to the cache if `fetch` is true.
     *
     * @param fs Filesystem object.
     * @param cache Cache object to change the cached block address of.
     * @param block New block to put in the cache.
     * @param write_back Whether or not to write back cached data (if block isn't already in the cache).
     * @param fetch Whether or not to fetch the new block's data (if block isn't already in the cache).
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_exchangeCache(const LogFs *fs, LogFsCache *cache, uint32_t block, bool write_back, bool fetch);

    /**
     * Sync a cache (write its data to disk).
     *
     * @param fs Filesystem object.
     * @param cache Cache object to sync.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_syncCache(const LogFs *fs, const LogFsCache *cache);

    /**
     * Initialize a new pair object. Does not sync anything to disk.
     *
     * @param pair Pair object to initialize.
     * @param block Address of first block in the pair.
     */
    void disk_newPair(LogFsPair *pair, uint32_t block);

    /**
     * Fetch a pair's state from the disk.
     *
     * @param fs Filesystem object.
     * @param pair Pair object to fetch to.
     * @param block Address of first block in the pair.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_fetchPair(const LogFs *fs, LogFsPair *pair, uint32_t block);

    /**
     * Write data to a pair on the disk.
     *
     * @param fs Filesystem object.
     * @param pair Pair object to write to.
     * @param check_replace Whether or not to check if a block has exceeded the maximum number of write cycles, and
     * replace it if necessary.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_writePair(LogFs *fs, LogFsPair *pair, bool check_replace);

    /**
     * Read data from a pair on the disk.
     *
     * @param fs Filesystem object.
     * @param pair Pair object to read from.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair);

#ifdef __cplusplus
}
#endif
